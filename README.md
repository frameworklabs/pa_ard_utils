# pa_ard_utils

[proto_activities](https://github.com/frameworklabs/proto_activities) arduino utilities

A set of utility activites to support Arduino development.

Curently includes types and activities to support button debouncing and short, long, double press recognition.

## Usage

* Include the header file: `#include "pa_ard_utils.h"`
* Use the namespace: `using namespace proto_activities::ard_utils;`
* Define a PressRecognizer and Press variable: `pa_ctx(pa_use(PressRecognizer); Press press; ...)`
* Run the PressRecognizer - probably concurrent with other trails:
  ```
  pa_activity (MyActA, pa_ctx(pa_co_res(2); pa_use(PressRecognizer); pa_use(MyActB); Press press)) {
    pa_co(2) {
      pa_with (PressRecognizer, BUTTON_A_PIN, pa_self.press);
      pa_with (MyActB, pa_self.press);
    } pa_co_end
  } pa_end
  ``` 



