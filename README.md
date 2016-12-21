#Encoder
##This library is intended to drive a rotary encoder.

For now, it's based on polling the pins as often as possible. Further developpment may lead to interrupt based polling.


Simply import it in your Arduino Sketch, call `begin()` in the setup by passing the two pins the encoder is plugged on, and call update on each loop.


`getStep()` returns 1 or -1 each time the encoder is incremented. Sign gives the direction.

`reverse()` change the sign of the value getStep() returns.

`setDebounceDelay()` change the delay used for debounce. if not called, it will default at 1 millisecond.

Added to those basic function, you can also attach a function or method to the encoder. The goal is that you can change the behavior of your encoder during program. For exemple you may want to increase or decrease a led intensity when in the main loop, but navigate betwen menus when you are on menu mode. Attaching a function make this posible, by attaching a new function whenever you want.

`exec()` calls the function that has benn attached to the encoder.

`attach()` is used to attach a function to the encoder. This function can be defined in global space, or can be part of an instance of a class.

`detach()` frees up the function, so following calls to `exec()` will have no effect. You don't have to call this before to change the function attached. Just call `attach()` again with a new function.


You are free to use this library for wathever project your working on.