# Mandelbrot set
A simple implementation of the mandelbrot set using opengl.
`master` contains an imlementation using `float32` allowing for less zoom while `double` contains an implementation using `float64` allowing for more zoom at the cost of performance. 
  * for mac: you may face difficulties running the `float64` version so it's best to stick to the regular version.
  * on linux: you may face weird bugs resulting from the fact that the application enters fullscreen mode. to fix this just replace `glfwGetPrimaryMonitor()` on line `50 of `main.cpp` with `NULL`.
  * on windows: make sure that you run it with your best graphics card, otherwise the framerat ewill be horrible.
  * for advanced users: you can change the fidelity and colouring of the image in `shaders/mandelbrot.frag`: 
      * on line 56 you can change `max_i` to increase the nuber of iteration and thus the accuracy of the approximation which is the final image (make note that the value `max_i` is inversely proportional to the framerate).
      * on line 68 you can change the inputs of the HLSToRGB function to change the colouring.
      * make sure to always reconfigure the cmake project to apply your changes to the shader files
# installation
  1. make sure you have cmake as well as a c++ compiler installed
  2. clone your prefered branch with `git clone`
  3. get the dependencies with `git submodule init` and `git submodule update`
  4. build using cmake
  5. enjoy
