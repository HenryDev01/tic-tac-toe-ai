/*
Neural Network for Tic Tac Toe

1. Data Preparation
- Extract data from datafile 
- Encode 'x', 'o', 'b' inputs to numerical values: 1, -1, 0
- Encode 'postitive', 'negative' targets to numerical values: 1, 0
- Shuffle data randomly due to positive and negative data separated in first and second halves
- Split data into 80% and 20% for train test sets

2. Neural Network Building
- Input layer: 9 neurons for each tic tac toe board space
- Hidden layer: 10 neurons
- Output layer: 1 neuron for positive/negative target prediction

3. Neural Network Training
- Stochastic Gradient Descent optimisation algorithm
- Foward Propagation with Sigmoid Activation
- Backward Propagation with Sigmoid Derivative
- 10 Epochs

4. Neural Network Evaluation
- Predict game win/loss on test data
- Confusion Matrix to show TP TN FP FN
- Accuracy: ((TP + TN) / total test set) * 100 **(evaluate function may not return same accuracy value if new model is created due to random shuffling of data)**
- Refer to Confusion_Matrix.png in data folder for example output

5. Interface for testing
- Game loop in terminal
- Allow player to input 'x' first
- Switch to AI
- AI will simulate all possible next move and choose the best one
- Return best move back to game function and input 'o' before switching to player
- Repeat until win/loss/tie

Possible improvements
1. ReLU or Tanh Activation Functions
2. Add More Layers or Nodes
3. Learning Rate Tuning e.g. between 0.001 and 0.1
4. More training data for all possible board moves

*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define INPUT_NODES 9
#define HIDDEN_NODES 10
#define OUTPUT_NODES 1
#define LEARNING_RATE 0.1
#define TRAINING_EPOCHS 10
#define BIAS_RANGE 0.01
#define TRAINING_DATA_FILE "./data/tic-tac-toe.data"
#define WEIGHTS_FILE "neural_network_weights.bin"
#define MAX_SAMPLES 958
#define PLAYER_SYMBOL 1  // Representing player with 'O' as -1
#define AI_SYMBOL -1       // Representing AI with 'X' as 1

// Structures
typedef struct {
    double input[INPUT_NODES];
    double hidden[HIDDEN_NODES];
    double output[OUTPUT_NODES];
    double weights_input_hidden[INPUT_NODES][HIDDEN_NODES];
    double weights_hidden_output[HIDDEN_NODES][OUTPUT_NODES];
    double biases_hidden[HIDDEN_NODES];
    double bias_output[OUTPUT_NODES];
} NeuralNetwork;

typedef struct {
    double input[INPUT_NODES];
    double target;
} DataSample;

typedef struct {
    DataSample samples[MAX_SAMPLES];
    int count;
} Dataset;

// Function prototypes
void initialize_network(NeuralNetwork *nn);
void save_weights(NeuralNetwork *nn);
int load_weights(NeuralNetwork *nn);
double sigmoid(double x);
double sigmoid_derivative(double x);
void forward_propagation(NeuralNetwork *nn, double inputs[INPUT_NODES]);
void sgd_backpropagation(NeuralNetwork *nn, double inputs[INPUT_NODES], double target);
void train(NeuralNetwork *nn, Dataset *train);
double predict(NeuralNetwork *nn, double inputs[INPUT_NODES]);
void load_data(const char *filename, Dataset *dataset);
void shuffle_data(Dataset *dataset);
void split_data(Dataset *dataset, Dataset *train_set, Dataset *test_set);
void evaluate_model_train(NeuralNetwork *nn, Dataset *train_set);
void evaluate_model_test(NeuralNetwork *nn, Dataset *test_set);
void display_board(int board[9]);
int check_win(int board[9]);
bool is_full(int board[9]);
int ai_move(NeuralNetwork *nn, int board[9]);
void play_game(NeuralNetwork *nn);

// Initialize weights with random values and biases to 0
void initialize_network(NeuralNetwork *nn) {
    srand(time(NULL));
    for (int i = 0; i < INPUT_NODES; i++) {
        nn->biases_hidden[i] = 0.0;
        for (int j = 0; j < HIDDEN_NODES; j++) {
            nn->weights_input_hidden[i][j] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
    }
    for (int i = 0; i < OUTPUT_NODES; i++) {
        nn->bias_output[i] = 0.0;  // Initialize output biases to 0
        for (int j = 0; j < HIDDEN_NODES; j++) {
            nn->weights_hidden_output[j][i] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
    }
}

// Save weights to binary file
void save_weights(NeuralNetwork *nn) {
    FILE *file = fopen(WEIGHTS_FILE, "wb");
    if (file == NULL) {
        perror("Error opening file to save weights");
        return;
    }
    fwrite(nn->weights_input_hidden, sizeof(double), INPUT_NODES * HIDDEN_NODES, file);
    fwrite(nn->weights_hidden_output, sizeof(double), HIDDEN_NODES * OUTPUT_NODES, file);
    fwrite(nn->biases_hidden, sizeof(double), HIDDEN_NODES, file);
    fwrite(nn->bias_output, sizeof(double), OUTPUT_NODES, file);
    fclose(file);
    printf("Neural network weights saved to %s\n", WEIGHTS_FILE);
}

// Load weights
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

// Sigmoid activation function
double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

// Derivative of sigmoid function
double sigmoid_derivative(double x) {
    return x * (1 - x);
}

// Forward Propagation Function
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

// Backpropagation and weight update using Stochastic Gradient Descent
void sgd_backpropagation(NeuralNetwork *nn, double inputs[INPUT_NODES], double target) {
    double output_error[OUTPUT_NODES];
    double hidden_error[HIDDEN_NODES];

    // Calculate the error for the output layer
    for (int i = 0; i < OUTPUT_NODES; i++) {
        output_error[i] = (target - nn->output[i]) * sigmoid_derivative(nn->output[i]);
    }

    // Calculate the error for the hidden layer (backpropagate error)
    for (int i = 0; i < HIDDEN_NODES; i++) {
        hidden_error[i] = 0.0;
        for (int j = 0; j < OUTPUT_NODES; j++) {
            hidden_error[i] += output_error[j] * nn->weights_hidden_output[i][j];
        }
        hidden_error[i] *= sigmoid_derivative(nn->hidden[i]);
    }

    // Update the weights and biases for the output layer (hidden to output)
    for (int i = 0; i < HIDDEN_NODES; i++) {
        for (int j = 0; j < OUTPUT_NODES; j++) {
            nn->weights_hidden_output[i][j] += LEARNING_RATE * output_error[j] * nn->hidden[i];
        }
    }
    for (int i = 0; i < OUTPUT_NODES; i++) {
        nn->bias_output[i] += LEARNING_RATE * output_error[i];
    }

    // Update the weights and biases for the hidden layer (input to hidden)
    for (int i = 0; i < INPUT_NODES; i++) {
        for (int j = 0; j < HIDDEN_NODES; j++) {
            nn->weights_input_hidden[i][j] += LEARNING_RATE * hidden_error[j] * inputs[i];
        }
    }
    for (int i = 0; i < HIDDEN_NODES; i++) {
        nn->biases_hidden[i] += LEARNING_RATE * hidden_error[i];
    }
}

// Train the neural network on a single data point
void train(NeuralNetwork *nn, Dataset *train_set) {
    for (int epoch = 0; epoch < TRAINING_EPOCHS; epoch++) {
        // Iterate through each training sample
        for (int i = 0; i < train_set->count; i++) {
            // Forward propagate the input
            forward_propagation(nn, train_set->samples[i].input);

            // Perform backpropagation and update weights using SGD
            sgd_backpropagation(nn, train_set->samples[i].input, train_set->samples[i].target);
        }
    }
}

// Predict the output
double predict(NeuralNetwork *nn, double inputs[INPUT_NODES]) {
    forward_propagation(nn, inputs);
    return nn->output[0]; // Return prediction score
}

// Load data into Dataset structure
void load_data(const char *filename, Dataset *dataset) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    dataset->count = 0;
    char line[50];

    // Read each line from the file
    while (fgets(line, sizeof(line), file) && dataset->count < MAX_SAMPLES) {
        // Tokenize the line using commas
        char *token = strtok(line, ",");
        int index = 0;

        while (token != NULL) {
            if (index < INPUT_NODES) {
                // Encode board state into input array
                dataset->samples[dataset->count].input[index] = (token[0] == 'x') ? 1.0 : (token[0] == 'o') ? -1.0 : 0.0;
            } else {
                // Encode pos / neg state
                if (strcmp(token, "positive\n") == 0 || strcmp(token, "positive") == 0) {
                    dataset->samples[dataset->count].target = 1.0; // Win corresponds to 1
                } else if (strcmp(token, "negative\n") == 0 || strcmp(token, "negative") == 0) {
                    dataset->samples[dataset->count].target = 0.0; // Loss corresponds to 0
                }
            }
            token = strtok(NULL, ",");
            index++;
        }

        dataset->count++;
    }

    fclose(file);
}

// Shuffle dataset
void shuffle_data(Dataset *dataset) {
    for (int i = dataset->count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        DataSample temp = dataset->samples[i];
        dataset->samples[i] = dataset->samples[j];
        dataset->samples[j] = temp;
    }
}

// Split dataset into training and testing sets
void split_data(Dataset *dataset, Dataset *train_set, Dataset *test_set) {
    int train_size = (int)(dataset->count * 0.8);
    train_set->count = train_size;
    test_set->count = dataset->count - train_size;

    for (int i = 0; i < train_size; i++) {
        train_set->samples[i] = dataset->samples[i];
    }
    for (int i = 0; i < test_set->count; i++) {
        test_set->samples[i] = dataset->samples[train_size + i];
    }
}

// Evaluate the model's train performance
void evaluate_model_train(NeuralNetwork *nn, Dataset *train_set) {
    int true_positive = 0;
    int true_negative = 0;
    int false_positive = 0;
    int false_negative = 0;

    for (int i = 0; i < train_set->count; i++) {
        double prediction = predict(nn, train_set->samples[i].input);
        int predicted_class = (prediction >= 0.5) ? 1 : 0; // Convert to binary classification

        if (predicted_class == 1 && train_set->samples[i].target == 1.0) {
            true_positive++;
        } else if (predicted_class == 0 && train_set->samples[i].target == 0.0) {
            true_negative++;
        } else if (predicted_class == 1 && train_set->samples[i].target == 0.0) {
            false_positive++;
        } else if (predicted_class == 0 && train_set->samples[i].target == 1.0) {
            false_negative++;
        }
    }

    // Calculate accuracy
    int total_samples = train_set->count;
    double accuracy = (double)(true_positive + true_negative) / total_samples * 100.0;

    // Print confusion matrix and accuracy
    printf("\nConfusion Matrix for train data:\n");
    printf("                 Predicted Positive    Predicted Negative\n");
    printf("Actual Positive      %d                     %d\n", true_positive, false_negative);
    printf("Actual Negative      %d                     %d\n", false_positive, true_negative);
    printf("\nTrain Accuracy: %.2f%%\n", accuracy);
}

// Evaluate the model's test performance
void evaluate_model_test(NeuralNetwork *nn, Dataset *test_set) {
    int true_positive = 0;
    int true_negative = 0;
    int false_positive = 0;
    int false_negative = 0;

    for (int i = 0; i < test_set->count; i++) {
        double prediction = predict(nn, test_set->samples[i].input);
        int predicted_class = (prediction >= 0.5) ? 1 : 0; // Convert to binary classification

        if (predicted_class == 1 && test_set->samples[i].target == 1.0) {
            true_positive++;
        } else if (predicted_class == 0 && test_set->samples[i].target == 0.0) {
            true_negative++;
        } else if (predicted_class == 1 && test_set->samples[i].target == 0.0) {
            false_positive++;
        } else if (predicted_class == 0 && test_set->samples[i].target == 1.0) {
            false_negative++;
        }
    }

    // Calculate accuracy
    int total_samples = test_set->count;
    double accuracy = (double)(true_positive + true_negative) / total_samples * 100.0;

    // Print confusion matrix and accuracy
    printf("\nConfusion Matrix for Test Data:\n");
    printf("                 Predicted Positive    Predicted Negative\n");
    printf("Actual Positive      %d                     %d\n", true_positive, false_negative);
    printf("Actual Negative      %d                     %d\n", false_positive, true_negative);
    printf("\nTest Accuracy: %.2f%%\n", accuracy);
}

// Display the Tic-Tac-Toe board
void display_board(int board[9]) {
    char symbols[3] = {'O', ' ', 'X'};
    printf("\nBoard:\n");
    for (int i = 0; i < 9; i++) {
        printf(" %c ", symbols[board[i] + 1]);
        if (i % 3 < 2) printf("|");
        if (i % 3 == 2 && i < 8) printf("\n---|---|---\n");
    }
    printf("\n");
}

// Check for a win on the board
int check_win(int board[9]) {
    int win_patterns[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // Columns
        {0, 4, 8}, {2, 4, 6}              // Diagonals
    };

    for (int i = 0; i < 8; i++) {
        int *p = win_patterns[i];
        if (board[p[0]] != 0 && board[p[0]] == board[p[1]] && board[p[1]] == board[p[2]]) {
            return board[p[0]];  // Return the winner
        }
    }
    return 0;  // Draw
}

// Check if the board is full if no winner
bool is_full(int board[9]) {
    for (int i = 0; i < 9; i++) {
        if (board[i] == 0) return false;
    }
    return true;
}

// Model predicts all possible next moves and picks the one with the best score
int ai_move(NeuralNetwork *nn, int board[9]) {
    int best_move = -1;
    double best_score = -1.0;

    for (int i = 0; i < 9; i++) {
        if (board[i] == 0) {  // Empty cell
            board[i] = AI_SYMBOL;

            // Convert board to inputs for the neural network
            double inputs[INPUT_NODES];
            for (int j = 0; j < 9; j++) {
                inputs[j] = (board[j] == AI_SYMBOL) ? 1.0 : (board[j] == PLAYER_SYMBOL ? -1.0 : 0.0);
            }

            // Forward propagate to get prediction score
            double score = predict(nn, inputs);

            // Revert the move
            board[i] = 0;

            // Select the move with the highest score
            if (score > best_score) {
                best_score = score;
                best_move = i;
            }
        }
    }
    return best_move;
}

// Quick terminal gui for testing
void play_game(NeuralNetwork *nn) {
    int board[9] = {0};  // 0 = empty, 1 = AI (X), -1 = Player (O)
    int current_player = PLAYER_SYMBOL;

    while (true) {
        display_board(board);

        // Check for winner
        int winner = check_win(board);
        if (winner != 0) {
            printf("%s wins\n", (winner == PLAYER_SYMBOL) ? "Player" : "AI");
            break;
        } else if (is_full(board)) {
            printf("Draw\n");
            break;
        }

        // Player's turn
        if (current_player == PLAYER_SYMBOL) {
            int move;
            printf("Enter your move (1-9): ");
            if (scanf("%d", &move) != 1 || move < 1 || move > 9 || board[move - 1] != 0) {
                printf("Invalid move. Try again.\n");
                while (getchar() != '\n');  // Clear input buffer
                continue;
            }
            board[move - 1] = PLAYER_SYMBOL;
        }
        // AI's predicts best move
        else {
            printf("AI's turn\n");
            int move = ai_move(nn, board);
            board[move] = AI_SYMBOL;
        }

        // Switch player
        current_player = -current_player;
    }
}

int main() {
    NeuralNetwork nn;
    Dataset dataset;

    // Load dataset
    load_data(TRAINING_DATA_FILE, &dataset);

    // Shuffle and split the dataset randomly into train and test sets
    shuffle_data(&dataset);
    Dataset train_set, test_set;
    split_data(&dataset, &train_set, &test_set);

    // Try loading saved weights
    if (!load_weights(&nn)) {
        // If no weights loaded, create and train the model
        initialize_network(&nn);

        // Train the neural network
        train(&nn, &train_set);

        // Save the trained weights for future use
        save_weights(&nn);
        
    }

    // Evaluate the model's performance on the train set (results differ due to randomised train and test dataset)
    evaluate_model_train(&nn, &train_set);

    // Evaluate the model's performance on the test set (results differ due to randomised train and test dataset)
    evaluate_model_test(&nn, &test_set);


    // Test the game in terminal
    play_game(&nn);

    return 0;
}