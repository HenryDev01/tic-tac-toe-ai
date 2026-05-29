#include "game_logic/game_logic.h"
#include "render/render.h"
#include "menu/menu.h"
#include "gui/gui.h"
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include<time.h>


void createBoardWith(SDL_Renderer*renderer, Game*game)
{   
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_NONE);  // set property like alpha in rgb to none
    SDL_SetRenderDrawColor(renderer, 255, 255, 255,255); // set the color for drawing board
    int x = width/8; 
    int y = 0;
    for(int r =0; r<9;r++) //iterate through 9 times to create 3x3 board
    {
       
        if(r % 3 == 0) //create new row when 1 row is completed
        {
            x = width/8;
            y +=140;
        }
        rects[r] = (struct SDL_Rect){.x = x, .y= y, .w = 240, .h =140};
        SDL_RenderDrawRect(renderer,&rects[r]); //draw rect
        if(game->state != WIN && game->state != TIE && game->state != ANIMATING)  //render the rectangle with delay of 50 sec to simulate animation effect. render visually with animation only during initalization phase
        {
            SDL_RenderPresent(renderer);
            SDL_Delay(50);
        }
        x+= 240;
    }

}
void resetBoard(SDL_Renderer*renderer,Game*game, Score*score)
{   
    // this function reintinializes and re-render components after a win.
    // after a win you need to clear the board.

    SDL_SetRenderTarget(renderer,NULL);
    //clear board & score
    game->totalMoveMade = 0;
    for(int i = 0, n = 9; i<9; i++)
    {
        game->board[i] = 0;

        if(i % 3 == 0)
            score->rowsPoint[i/3] = 0;
        
        if(i < 3)
            score->columnPoints[i] = 0;
    }
    score->rightDiagonalPoint = 0;
    score->leftdiagonalPoint = 0;
    score->winner = 0;

    //reset score back to 0 after best of 5
    if(score->player_o_points == 5 || score->player_x_points == 5)
    {
        score->player_o_points = 0;
        score->player_x_points = 0;
        score->tie_points = 0;

    }

    //reset
    SDL_RenderClear(renderer);
    //reinitialize the board 
    initBackground("./assets/bg3.bmp",renderer);
    createBoardWith(renderer,game);
    initPlayerScore(renderer,score,game);
    
    game->state = RUNNING;
}
void switchPlayer(Game*game)
{   
    // end function if player has not made a move on the board
    if(!game->playerHasMoved)
        return;

    // switch player.
    if(game->currentPlayer != PLAYER_X)
    {
        game->currentPlayer =  PLAYER_X;
        return;
    }
    game->currentPlayer = PLAYER_O;
}

void showPlayerTurn(SDL_Renderer*renderer, Game*game, Score*score)
{   // move clear and render
    SDL_Surface*surface;
    if(game->mode == 0) // 0 means player vs player
        surface = (game->currentPlayer == PLAYER_O) ? SDL_LoadBMP("./assets/O_Turn.bmp") : SDL_LoadBMP("./assets/x_turn.bmp");
    else //everything else is ai vs player
        surface = (game->currentPlayer == PLAYER_O) ? SDL_LoadBMP("./assets/AI_turn.bmp") : SDL_LoadBMP("./assets/your_turn.bmp");

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    if(!game->playerHasMoved && game->state != INITIALIZING)
        return;

    int previousState = game->state;
    game->state = ANIMATING;
    //initial position
    SDL_Rect playerRect = {.x = 320,.y=height,.w=350,.h=200};
    int targetPosition = height -120;

    SDL_Rect exitBtn= {5, 5, 90,30};
    SDL_Color color = {0, 174, 239,255};

    // initialiing surface and texture here instead of the loop to lessen load of the program since surface and texture involve computer's CPU.
    TTF_Font * font = TTF_OpenFont("./Font/MouseMemoirs-Regular.ttf",50);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font,"Return",color);
    SDL_Texture*texturetext = SDL_CreateTextureFromSurface(renderer,textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Surface*bgSurface = SDL_LoadBMP("./assets/bg3.bmp");
    SDL_Texture*bgTexture = SDL_CreateTextureFromSurface(renderer,bgSurface);
    SDL_FreeSurface(bgSurface);

    //iteration to animate the show player sign board.
    //as you move each frame you need clear the render
    // therefore we need reinitialize the neccesary components again
    for(int i = playerRect.y;targetPosition < i; i -= 15)
    {   
        playerRect.y = i;
       // SDL_SetRenderTarget(renderer,texture);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,bgTexture,NULL,NULL);
        createBoardWith(renderer,game);
        initPlayerScore(renderer,score,game); 
        drawAll(renderer,game,rects);
        SDL_RenderCopy(renderer,texturetext,NULL,&exitBtn);
        SDL_RenderCopy(renderer,texture,NULL,&playerRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(5);

    }
    
    //free memory
    SDL_DestroyTexture(texturetext);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(texture);
   game->state = previousState;
}

void showWinScreen(Game*game,Score*score,SDL_Renderer*renderer)
{   
    //default best of 5

    // early exit 
     if(game->state != WIN )
        return;
    
    // validation check
    if(score->player_o_points != 5 && score->player_x_points !=5)
        return;
    
    //load up neccessary resources for the text
    int textWidth = 0, textHeight = 0;
    TTF_Font * font = TTF_OpenFont("./Font/MouseMemoirs-Regular.ttf",50);
    SDL_Color color = {255,255,255,255};
    SDL_Surface*textSurface;

    // changes the text according to player.
    if(score->winner == PLAYER_X)
    {    textSurface = TTF_RenderText_Solid(font,"Score of 5 reached. X WON",color);}
    else
    {    textSurface = TTF_RenderText_Solid(font,"Score of 5 reached. O WON",color);}

    SDL_Texture*texturetext = SDL_CreateTextureFromSurface(renderer,textSurface);

    //position the text as rectangle
    SDL_QueryTexture(texturetext, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect textRect = {(width - textWidth) / 2, (height - textHeight) / 2, textWidth , textHeight};


    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);


    
        // animate a fade in background by incrementing the alpha value of rbga
       for (int alpha = 0; alpha <= 30; alpha +=5)  {

    
        SDL_SetRenderDrawColor(renderer,139, 69, 19, alpha);
        SDL_RenderFillRect(renderer,NULL);
      

        SDL_SetTextureAlphaMod(texturetext, 255);  // Keep text fully opaque
        SDL_RenderCopy(renderer, texturetext, NULL, &textRect);
        SDL_Delay(50);

        SDL_RenderPresent(renderer);
    }
    
   //free 
    TTF_Quit();
    SDL_Delay(2000);
    SDL_DestroyTexture(texturetext);




}

void updateScore(Game*game, Score*score) // detect winner function
{   
    // O(1) way of detecting winner in tic tac toe
    // does not use iteration to find the winning positions
    
    
    // early exit
    if(!game->playerHasMoved)
        return;

    int row = game->lastMovePosition / 3;
    int column = game->lastMovePosition % 3;

    //update row. column and diagonal with score up to cumulative score of 3 for player_x
    if(game->currentPlayer == PLAYER_X)
    {   
        updatePlayerX(game,score);

    // else ifupdate row with score up to cumulative score up to -3 for player_o  
    }else if(game->currentPlayer == PLAYER_O) 
    {   
        updatePlayerO(game,score);
    }

    // announce the winner once the cumulative score is 3 or -3
    if(score->rowsPoint[row] == DIMENSION || score->rowsPoint[row] == DIMENSION*-1 || score->columnPoints[column] ==  DIMENSION || score->columnPoints[column] == DIMENSION*-1 || score->leftdiagonalPoint ==  DIMENSION || score->leftdiagonalPoint == DIMENSION*-1 || score->rightDiagonalPoint == DIMENSION || score->rightDiagonalPoint == DIMENSION*-1)
    {
        score->winner = game->currentPlayer; //winner will be the current player who made the last turn in the board
        game->state = WIN; // game set to win

    }// else if all move are made and no winner found then game is set to tie
    else if( (game->totalMoveMade == DIMENSION*DIMENSION) && (score->rowsPoint[row] != DIMENSION || score->rowsPoint[row] != DIMENSION*-1 || score->columnPoints[column] !=  DIMENSION || score->columnPoints[column] != DIMENSION*-1 || score->leftdiagonalPoint !=  DIMENSION || score->leftdiagonalPoint != DIMENSION*-1 || score->rightDiagonalPoint != DIMENSION || score->rightDiagonalPoint != DIMENSION*-1))
     {
         game->state = TIE;  // set to tie
         score->tie_points +=1;
     }  
    
    // tally score for the winner if there is a winner found.
    if(score->winner == PLAYER_X)
        score->player_x_points +=1;
    else if(score->winner == PLAYER_O)
        score->player_o_points +=1;


    
   
  
}

void updatePlayerX(Game*game, Score*score) //helper function for updateScore()
{
    int row = game->lastMovePosition / 3;
    int column = game->lastMovePosition % 3;
    if(row==column )
        score->leftdiagonalPoint +=1;
    
    if(row + column == DIMENSION - 1)
        score->rightDiagonalPoint +=1;

    score->rowsPoint[row]+=1;
    score->columnPoints[column] +=1;
}
void updatePlayerO(Game*game, Score*score) //helper function for updateScore()
{
    int row = game->lastMovePosition / 3;
    int column = game->lastMovePosition % 3;

    if(row==column )
    score->leftdiagonalPoint -=1;

    if(row + column == DIMENSION - 1)
        score->rightDiagonalPoint -=1;

    score->rowsPoint[row] -=1;
    score->columnPoints[column] -=1;
}
void updateBoardByPosition(SDL_Point mousePosition, Game *game)
{   
    //set playerHasMoved = false. so that if user didnt perform a valid the subsequent function will check for this and then exit the function.
    game->playerHasMoved = false;
    for(int r = 0; r<9; r++)
    {  
        //checks if user click within the boundary of the board
       if(SDL_PointInRect(&mousePosition,&rects[r]) && game->board[r] == 0)
       {
            game->board[r] = game->currentPlayer;
            game->totalMoveMade +=1;
            game->lastMovePosition = r; //last updated position. other functions can make use of this instead of iterating through the entire board.
            game->playerHasMoved = true;
            break;
       }
    }
}
void highlightWinRect(SDL_Renderer*renderer, Score*score,Game*game)
{      
    // early exit
    if(game->state != WIN)
        return;

 
     SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    if(score->winner == PLAYER_X)    
        SDL_SetRenderDrawColor(renderer,60, 162, 220,60); //set color for player x
    else
        SDL_SetRenderDrawColor(renderer,190, 125, 118,60); // set color for player o
    
    // highlight left diagonal winning poisiotions
    if(score->leftdiagonalPoint == DIMENSION || score->leftdiagonalPoint == DIMENSION*-1)
    {   
        int leftDiagonalRow = 0;

        for(int i = 0;i<DIMENSION;i++)
        {   
            SDL_RenderFillRect(renderer,&rects[leftDiagonalRow]);
            leftDiagonalRow += DIMENSION;
            leftDiagonalRow +=1;
            SDL_RenderPresent(renderer);
            SDL_Delay(500);

        }
    //highlight right diagonal winning positions
    }else if(score->rightDiagonalPoint == DIMENSION || score->rightDiagonalPoint == DIMENSION*-1)
    {
        int rightDiagonalRow = DIMENSION - 1;
        for(int i = 0;i<DIMENSION;i++)
        {   
            SDL_RenderFillRect(renderer,&rects[rightDiagonalRow]);
            rightDiagonalRow += (DIMENSION-1);
            SDL_RenderPresent(renderer);
            SDL_Delay(500);

        }

    }else
    {   
        //highlight row or column if winning posistions are found there.

        //row and column
        int row, column;
        bool rowFound = false;
        for(int i = 0; i<DIMENSION;i++)
        {   
            if(score->rowsPoint[i] == DIMENSION || score->rowsPoint[i] == DIMENSION*-1)
            {
                row =i;
                rowFound = true;
                break;
            }else if(score->columnPoints[i] == DIMENSION || score->columnPoints[i] == DIMENSION*-1)
            {
                column = i;
                break;
            }
        }
        if(rowFound)
        {
            int rowIndex = row*DIMENSION;
            for(int i = rowIndex; i<rowIndex+DIMENSION;i++)
            {   
                SDL_RenderFillRect(renderer,&rects[i]);
                SDL_RenderPresent(renderer);
                SDL_Delay(500);
            }
            return;
        }

        for(int i = column; i < DIMENSION*DIMENSION;i+=DIMENSION)
        {
            SDL_RenderFillRect(renderer,&rects[i]);
            SDL_RenderPresent(renderer);
            SDL_Delay(500);
        }
    }
  
}
void computerMove(Game*game)
{   
    game->playerHasMoved = false;
    int currentBestScore = -100;
    int bestMove = -1;

    //this is for imperfect minimax if the game mode detected to be imperfect minimax.
    if(game->mode==2)
    {
       random_move(game);
       
       if(game->playerHasMoved) {
            printf("random move made\n");
            return; //if computer made a random move then end this function else perform minimax.

       }

    }

    int moveScore;
    for(int i =0,n = DIMENSION*DIMENSION; i < n; i++)
    {
        if(game->board[i] != 0)
            continue;

        game->board[i] = CPU;
        //generate minimax tree based on whether the mode is perfect or imperfect.
        if(game->mode == 1) // if player selects perfect minimax, the game mode is 1.
        {               
             moveScore = minimax(game->board, PLAYER_X, -100, 100,9); //perfect minimax
        }else if(game->mode == 2) //if player selects imperfect minimax, the game mode is 2.
        {      
             moveScore = minimax(game->board, PLAYER_X, -100, 100,5);  //imperfect with depth of 5.
        }
       
        game->board[i] = 0; // reset back to original board

        if(moveScore > currentBestScore)
        {   
            printf("%i\n", moveScore);
            currentBestScore = moveScore;
            bestMove = i;
    
        }


    }
    
    game->board[bestMove] = CPU;
    game->totalMoveMade +=1;
    game->lastMovePosition = bestMove;
    game->playerHasMoved = true;

}

void random_move(Game*game)
{
        srand(time(0)); //generate set of unique random value.
        int bestMove;
        if (rand() % 10 == 0) // 10% chance to plot at random position
        {
            int availableMoves[9];
            int moveCount = 0;

            for (int i = 0; i < 9; ++i) // loop all cells
            {
                if (game->board[i] == 0) // check empty
                    availableMoves[moveCount++] = i; // create an array with posistion available
                
            }

            // Choose a random move from available ones
            if (moveCount > 0)
                bestMove = availableMoves[rand() % moveCount]; //best move is one of the random available cell
            
            //update game
            printf("bestmove: %i\n", bestMove);
            game->board[bestMove] = CPU;
            game->totalMoveMade +=1;
            game->lastMovePosition = bestMove;
            game->playerHasMoved = true;
        }
}

int minimax(int board[DIMENSION*DIMENSION], int player, int alphaScore, int betaScore, int depth)
{   // alphaScore = best nextPlayerMoveEval of computer (Get max)
    // betaScore = best nextPlayerMoveEval of human (Get min)
    int winner = win(board);
    if(winner !=0 || depth == 0)
    {
        if (winner == CPU)
            return 10; // AI win is 10
        else
            return -10; // player win is -10
    }
    
       // Check for a draw
    int isDraw = 1; // default isDraw is True
    for (int i = 0; i < 9; ++i)
    {
        if (board[i] == 0) // check for empty cell
        {
            isDraw = 0; // if there is empty cell, means game is not end. Draw = False
            break;
        }
    }
    if (isDraw)  
        return 0; // Draw nextPlayerMoveEval is 0
    

        if(player == CPU)     // check player, when computer is playing (Maximizer), only Alpha is changed
        {   
            int max_score = -100;  // evaluation for max player (computer)
            for(int i =0,n=DIMENSION*DIMENSION;i<n;i++)
            {
                
                if(board[i] != 0)
                    continue;

                board[i] = player; // make a step when board is empty
                int nextPlayerMoveEval = minimax(board,PLAYER_X,alphaScore,betaScore,depth-1); //change player
                 // if somone win, will return a nextPlayerMoveEval
                // else, will continue recursive funtion until someone wins
             
                board[i] = 0; //undo the step to return to its original board

                if(max_score < nextPlayerMoveEval)
                    max_score = nextPlayerMoveEval; // find max between max result and calculated nextPlayerMoveEval

                if(alphaScore < nextPlayerMoveEval)
                    alphaScore = nextPlayerMoveEval; // find max to alpha and calculated nextPlayerMoveEval
                
                if(alphaScore >= betaScore) // stop or cut the minimax branch, when alpha(computer) is greater than beta
                    break; // does not need to continue for the computer to win.
            }
         
            return max_score;
            
        }
        else // minimizer's turn (human)
        {   

            int min_score = 100; 
            for(int i =0,n=DIMENSION*DIMENSION;i<n;i++)
            {      
                if(board[i] != 0)
                    continue;
                board[i] = player; //similar to CPU. make a step.
                int nextPlayerMoveEval = minimax(board,CPU,alphaScore,betaScore, depth-1); //change player

                board[i] = 0; //undo to its original state.
                if(min_score > nextPlayerMoveEval)
                    min_score = nextPlayerMoveEval; // find max between max result and calculated nextPlayerMoveEval

                if(betaScore > nextPlayerMoveEval)
                    betaScore = nextPlayerMoveEval; // find max to alpha and calculated nextPlayerMoveEval
                
                if(alphaScore >= betaScore)  // stop or cut the minimax branch, when alpha(computer) is greater than beta
                    break; //does not need to continue for the computer to win.
            }
            return min_score;
            
        }
           
}



int win(int board[9])
{
    // determines if a player has won, returns 0 otherwise.
    int wins[8][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},
        {0, 4, 8},
        {2, 4, 6}}; // board index store in array

    for (int i = 0; i < 8; ++i)
    {
        int getZeroIndex = wins[i][0];
        int getOneIndex = wins[i][1];
        int getTwoIndex = wins[i][2];
        if (board[getZeroIndex] != 0 && board[getZeroIndex] == board[getOneIndex] && board[getZeroIndex] == board[getTwoIndex])
            return board[getTwoIndex];
    }
    return 0;
}

// Function to load saved weights
int load_weights(NeuralNetwork *nn) {
    FILE *file = fopen(WEIGHTS_FILE, "rb");
    if (file == NULL) {
        printf("No saved weights found. Training will be required.\n");
        return 0; // File does not exist
    }
    fread(nn->weights_input_hidden, sizeof(double), INPUT_NODES * HIDDEN_NODES, file);
    fread(nn->weights_hidden_output, sizeof(double), HIDDEN_NODES * OUTPUT_NODES, file);
    fread(nn->biases_hidden, sizeof(double), HIDDEN_NODES, file);
    fread(nn->bias_output, sizeof(double), OUTPUT_NODES, file);
    fclose(file);
    printf("Neural network weights loaded from %s\n", WEIGHTS_FILE);
    return 1; // Successfully loaded
}

// Forward propagation
void forward_propagation(NeuralNetwork *nn, double inputs[INPUT_NODES]) {
    // Calculate hidden layer activations (weighted sum + activation)
    for (int i = 0; i < HIDDEN_NODES; i++) {
        nn->hidden[i] = nn->biases_hidden[i];
        for (int j = 0; j < INPUT_NODES; j++) {
            nn->hidden[i] += nn->weights_input_hidden[j][i] * inputs[j];
        }
        nn->hidden[i] = sigmoid(nn->hidden[i]);  // Apply sigmoid activation to hidden layer
    }
    
    // Calculate output layer activations (weighted sum + activation)
    for (int i = 0; i < OUTPUT_NODES; i++) {
        nn->output[i] = nn->bias_output[i];
        for (int j = 0; j < HIDDEN_NODES; j++) {
            nn->output[i] += nn->weights_hidden_output[j][i] * nn->hidden[j];
        }
        nn->output[i] = sigmoid(nn->output[i]);  // Apply sigmoid activation to output layer
    }
}

// Sigmoid activation function
double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

// Predict the output for given inputs
double predict(NeuralNetwork *nn, double inputs[INPUT_NODES]) {
    forward_propagation(nn, inputs);
    return nn->output[0];
}

// Model predicts all possible next moves and picks the one with the best score
int get_best_move(NeuralNetwork *nn, Game*game) {
    game->playerHasMoved = false;
    int best_move = -1;
    double best_score = -1.0;

    for(int i =0,n = DIMENSION*DIMENSION; i < n; i++) {

        if(game->board[i] != 0)
            continue;

        game->board[i] = NN;

        // Convert board to inputs for the neural network
        double inputs[INPUT_NODES];
        for (int j = 0; j < 9; j++) {
            inputs[j] = (game->board[j] == NN) ? 1.0 : (game->board[j] == PLAYER_X ? -1.0 : 0.0);
            printf("%d\n", inputs[j]);
        }

        double score = predict(nn, inputs);

        // Track the best move with the highest score
        if (score > best_score) {
            best_score = score;
            best_move = i;
        }

        printf("score: %f\n", score);
        printf("best score: %f\n", best_score);

        // Undo the simulated move
        game->board[i] = 0;

    }
    
    game->board[best_move] = NN;
    game->totalMoveMade +=1;
    game->lastMovePosition = best_move;
    game->playerHasMoved = true;
}
