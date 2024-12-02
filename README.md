# pa_ard_utils

[proto_activities](https://github.com/frameworklabs/proto_activities) arduino utilities

A set of utility activites to support Arduino development.

Curently includes types and activities to support:

* button debouncing
* short, long and double press recognition
* detecting and converting levels to edges and vice versa
* timing additions, delay_ms as activity

## Usage

* Include the header file: `#include "pa_ard_utils.h"`
* Use the namespace: `using namespace proto_activities::ard_utils;`
* Define a PressRecognizer and PressSignal variable: `pa_ctx(...; pa_use(PressRecognizer); pa_def_val_signal(Press, press))`
* Run the PressRecognizer - probably concurrently with other trails:
  ```
  pa_activity (MyActA, pa_ctx(pa_co_res(2); pa_signal_res; pa_use(PressRecognizer); pa_use(MyActB); pa_def_val_signal(Press, press))) {
    pa_co(2) {
      pa_with (PressRecognizer, BUTTON_A_PIN, pa_self.press);
      pa_with (MyActB, pa_self.press);
    } pa_co_end
  } pa_end
  ```
* In your activity (e.g. `MyActB` above), test the presence of a press by code like this: `press && (press.val() == Press::SHORT)`



