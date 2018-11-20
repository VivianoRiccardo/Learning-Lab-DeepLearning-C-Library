# Learning-Lab-C-Library:

Creating the library:

```
make
```

# Run a file.c file using llab:

Link in your file.c the header "/directory-path/llab.h"

and then

```
gcc -o file  -L /path-to-the-llab.a-library-created-with-the-makefile/ file.c -lllab -lm
```

# Current Roadmap:

- fully-connected-layers feed forward (20/11/2018)
- fully-connected-layers backpropagation (20/11/2018)
- nesterov momentum (20/11/2018)
- adam optimization algorithm (not implemented yet in model_update function) (20/11/2018)
- fully-connected layers dropout (20/11/2018)
- convolutional layers feed forward (20/11/2018)
- convolutional layers backpropagation (20/11/2018)
- convolutional 2d max-pooling (20/11/2018)
- convolutional 2d avarage pooling (20/11/2018)
- convolutional 2d local response normalization (20/11/2018)
- convolutional padding (20/11/2018)
- fully-connected sigmoid activation (20/11/2018)
- fully-connected relu activation (20/11/2018)
- fully-connected softmax activation (20/11/2018)
- fully-connected tanh activation (20/11/2018)
- mse loss (20/11/2018)
- cross-entropy loss (20/11/2018)
- reduced cross-entropy form with softmax (20/11/2018)
- convolutional sigmoid activation (20/11/2018)
- convolutional relu activation (20/11/2018)
- convolutional tanh activation (20/11/2018)
- residual layers filled with convolutional layers (20/11/2018)
- residual layers feed-forward (20/11/2018)
- residual layers backpropagation (20/11/2018)
- model structure with fully-connected,convolutional,residual layers (20/11/2018)

# Future implementations

- Batch normalization
- Ridge regression (L2)
- Augmentation
- Recurrent layers
- LSTM layers
- DDQN easy implementation function
- Graphic test
- Support Vector Machine algorithms
- Decision tree
- Random forest
- OpenCl and Cuda implementation
- ...