#include <llab.h>

int main(){
    // 2 CONVOLUTIONAL LAYER:
    /* input 1° cl = 1*32*32, activation = RELU, normalization = GROUPED NORM, PADDING = 1
     * input 2° cl = pooling
     * */
    // 2 FULLY-CONNECTED LAYERS:
    /* input 1° fcl = 784, output = 100, activation = sigmoid,  dropout 0.3
     * input 2° fcl = 100, output = 10, activation = softmax, no dropout
     * mini batch = 10
     * nesterov momentum with momentum = 0.9
     * learning rate = 0.0003
     * l2 regularization with lambda = 0.001
     * epochs = 20
     * */
    srand(time(NULL));
    // Initializing Training resources
    int i,j,k,z,training_instances = 50000,input_dimension = 784,output_dimension = 10, middle_neurons = 100;
    int n_layers = 4;
    int batch_size = 10,threads = 4;
    int epochs = 20;
    unsigned long long int t = 1;

    char** ksource = (char**)malloc(sizeof(char*));
    char* filename = "../data/train.bin";
    int size = 0;
    char temp[2];
    temp[1] = '\0';
    // Model Architecture
    cl** cls = (cl**)malloc(sizeof(cl*)*2);
    cls[0] = convolutional(1,28,28,3,3,20,1,1,1,1,2,2,0,0,0,0,GROUP_NORMALIZATION,RELU,NO_POOLING,5,CONVOLUTION,0);
    cls[1] = convolutional(20,28,28,1,1,20,1,1,0,0,2,2,0,0,2,2,NO_NORMALIZATION,RELU,AVARAGE_POOLING,0,NO_CONVOLUTION,1);
    fcl** fcls = (fcl**)malloc(sizeof(fcl*)*2);
    fcls[0] = fully_connected(cls[1]->rows2*cls[1]->cols2*cls[1]->n_kernels,middle_neurons,2,DROPOUT,SIGMOID,0.3);
    fcls[1] = fully_connected(middle_neurons,output_dimension,3,NO_DROPOUT,SOFTMAX,0);
    model* m = network(n_layers,0,2,2,NULL,cls,fcls);
    model** batch_m = (model**)malloc(sizeof(model*)*batch_size);
    float** ret_err = (float**)malloc(sizeof(float*)*batch_size);
    for(i = 0; i < batch_size; i++){
        batch_m[i] = copy_model(m);
    }
    int ws = count_weights(m);
    float lr = 0.0003, momentum = 0.9, lambda = 0.0001;
    // Reading the data in a char** vector
    read_file_in_char_vector(ksource,filename,&size);
    float** inputs = (float**)malloc(sizeof(float*)*training_instances);
    float** outputs = (float**)malloc(sizeof(float*)*training_instances);
    // Putting the data in float** vectors
    for(i = 0; i < training_instances; i++){
        inputs[i] = (float*)malloc(sizeof(float)*input_dimension);
        outputs[i] = (float*)calloc(output_dimension,sizeof(float));
        for(j = 0; j < input_dimension+1; j++){
            temp[0] = ksource[0][i*(input_dimension+1)+j];
            if(j == input_dimension)
                outputs[i][atoi(temp)] = 1;
            else
                inputs[i][j] = atof(temp);
        }
    }
    
    printf("Training phase!\n");
    save_model(m,0);
    // Training
    for(k = 0; k < epochs; k++){
        if(k == 10)
            lr = 0.0001;
        else if(k == 15)
            lr = 0.00005;
        printf("Starting epoch %d/%d\n",k+1,epochs);
        // Shuffling before each epoch
        shuffle_float_matrices(inputs,outputs,training_instances);
        for(i = 0; i < training_instances/batch_size; i++){
            //printf("Mini batch number: %d\n",i+1);
            // Feed forward and backpropagation
            model_tensor_input_ff_multicore(batch_m,input_dimension,1,1,&inputs[i*batch_size],batch_size,threads);
            model_tensor_input_bp_multicore(batch_m,input_dimension,1,1,&inputs[i*batch_size],batch_size,threads,&outputs[i*batch_size],output_dimension,ret_err);
            // sum the partial derivatives in m obtained from backpropagation
            for(j = 0; j < batch_size; j++){
                sum_model_partial_derivatives(batch_m[j],m,m);
            }
            // update m, reset m and copy the new weights in each instance of m of the batch
            update_model(m,lr,momentum,batch_size,NESTEROV,NULL,NULL,NO_REGULARIZATION,ws,lambda,&t);
            reset_model(m);
            for(j = 0; j < batch_size; j++){
                paste_model(m,batch_m[j]);
                reset_model(batch_m[j]);
            }
            
        }
        // Saving the model
        save_model(m,k+1);
    }
    
    // Deallocating Training resources
    free(ksource[0]);
    free(ksource);
    free_model(m);
    for(i = 0; i < batch_size; i++){
        free_model(batch_m[i]);
    }
    free(batch_m);
    free(ret_err);
    for(i = 0; i < training_instances; i++){
        free(inputs[i]);
        free(outputs[i]);
    }
    free(inputs);
    free(outputs);
    
    // Initializing Testing resources
    model* test_m;
    char** ksource2 = (char**)malloc(sizeof(char*));
    char* filename2 = "../data/test.bin";
    int size2 = 0;
    int testing_instances = 10000;
    char temp2[256];
    read_file_in_char_vector(ksource2,filename2,&size);
    float** inputs_test = (float**)malloc(sizeof(float*)*testing_instances);
    float** outputs_test = (float**)malloc(sizeof(float*)*testing_instances);
    // Putting the data in float** vectors
    for(i = 0; i < testing_instances; i++){
        inputs_test[i] = (float*)malloc(sizeof(float)*input_dimension);
        outputs_test[i] = (float*)calloc(output_dimension,sizeof(float));
        for(j = 0; j < input_dimension+1; j++){
            temp[0] = ksource2[0][i*(input_dimension+1)+j];
            if(j == input_dimension)
                outputs_test[i][atoi(temp)] = 1;
            else
                inputs_test[i][j] = atof(temp);
        }
    }
    
    
    printf("Testing phase!\n");
    double error = 0;
    // Testing
    for(k = 0; k < epochs+1; k++){
        printf("Model N. %d/%d\n",k+1,epochs);
        // Loading the model
        char temp3[5];
        temp3[0] = '.';
        temp3[1] = 'b';
        temp3[2] = 'i';
        temp3[3] = 'n';
        temp3[4] = '\0';
        itoa(k,temp2);
        strcat(temp2,temp3);
        test_m = load_model(temp2);
        test_m->fcls[0]->dropout_flag = DROPOUT_TEST;
        test_m->fcls[0]->dropout_threshold = 0.7;
        for(i = 0; i < testing_instances; i++){
            // Feed forward
            model_tensor_input_ff(test_m,input_dimension,1,1,inputs_test[i]);
            for(j = 0; j < output_dimension; j++){
                error+=cross_entropy(test_m->fcls[1]->post_activation[j],outputs_test[i][j]);
            }
            reset_model(test_m);  
        }
        printf("Error: %lf\n",error);
        error = 0;
        free_model(test_m);
    }
    // Deallocating testing resources
    free(ksource2[0]);
    free(ksource2);
    for(i = 0; i < testing_instances; i++){
        free(inputs_test[i]);
        free(outputs_test[i]);
    }
    free(inputs_test);
    free(outputs_test);
}