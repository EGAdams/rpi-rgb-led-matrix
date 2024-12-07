# Plan for implementing a remote for a Tennis Scoring System

I have a remote control for a tennis scoring system with 4 pins for output.

For example:
- 0001 = Green Button Pressed
- 0010 = Red Button Pressed
- 0011 = Undo Button Pressed
- and 13 more for other things...

We read those 4 bits continually in our C++ code so that if there is a valid code, we take action.

We need to be able to use the available GPIO pins to get the binary signal from the remotes.

I have looked at the C++ code that controls the GPIO pins and I don't completely understand it yet.
What I did see is the part of the C++ code that is used map GPIO pins.
These 3 pins look unused to me:
```cpp
.p2_r2         = GPIO_BIT(26),
.p2_g2         = GPIO_BIT(16),
.p2_b2         = GPIO_BIT(21),
```
The problem is that these are not the 4 pins needed, so we need at least one more.

But since we are here, let's explore plan B a bit before we start diving into the internals.
If we confirm that these three pins are good to use, we could use the following Python code to operate a shift register to do what we want.  Here is the Python code:
```python
import RPi.GPIO as GPIO
import time

DATA_PIN  = 26  # Serial data out from shift register
CLOCK_PIN = 16  # Clock pin for the shift register
LATCH_PIN = 21  # Latch pin to load data

GPIO.setmode(GPIO.BCM)
GPIO.setup(DATA_PIN, GPIO.IN)
GPIO.setup(CLOCK_PIN, GPIO.OUT)
GPIO.setup(LATCH_PIN, GPIO.OUT)

def read_shift_register():
    GPIO.output(LATCH_PIN, 0)
    time.sleep(0.01)  # Small delay to stabilize
    GPIO.output(LATCH_PIN, 1)  # Load data from parallel to serial

    data = []
    for _ in range(8):  # Assuming 8 bits
        GPIO.output(CLOCK_PIN, 1)
        data.append(GPIO.input(DATA_PIN))
        GPIO.output(CLOCK_PIN, 0)
    return data

try:
    while True:
        data = read_shift_register()
        print(f"Shift Register Data: {data}")
        time.sleep(0.5)
except KeyboardInterrupt:
    print("Exiting...")
finally:
    GPIO.cleanup()
```

This might work, but let's continue on with plan A.
We are hooking this up the `https://www.electrodragon.com/product/rgb-matrix-panel-drive-board-raspberry-pi/` HUB75 driver HAT that will be running on top of a Raspberry Pi Zero 2w.

Since we already have the C++ library that controls the HUB75 matrix using the `https://github.com/hzeller/rpi-rgb-led-matrix` solution code, I would think that we could leverage parts of this library to control the GPIO pins coming from the HAT.


Since the hardware mapping type is set to "regular", the cpp snippet below is for the "regular" type.
## Hardware Mapping Source Code for your reference
```cpp
  #define GPIO_BIT(b) ((uint64_t)1<<(b))
  /*
   * The regular hardware mapping described in the wiring.md and used
   * by the adapter PCBs.
   */
  {
    .name          = "regular",

    .output_enable = GPIO_BIT(18),
    .clock         = GPIO_BIT(17),
    .strobe        = GPIO_BIT(4),

    /* Address lines */
    .a             = GPIO_BIT(22),
    .b             = GPIO_BIT(23),
    .c             = GPIO_BIT(24),
    .d             = GPIO_BIT(25),
    .e             = GPIO_BIT(15),  /* RxD kept free unless 1:64 */

    /* Parallel chain 0, RGB for both sub-panels */
    .p0_r1         = GPIO_BIT(11),  /* masks: SPI0_SCKL  */
    .p0_g1         = GPIO_BIT(27),  /* Not on RPi1, Rev1; use "regular-pi1" instead */
    .p0_b1         = GPIO_BIT(7),   /* masks: SPI0_CE1   */
    .p0_r2         = GPIO_BIT(8),   /* masks: SPI0_CE0   */
    .p0_g2         = GPIO_BIT(9),   /* masks: SPI0_MISO  */
    .p0_b2         = GPIO_BIT(10),  /* masks: SPI0_MOSI  */

    /* All the following are only available with 40 GPIP pins, on A+/B+/Pi2,3 */
    /* Chain 1 */
    .p1_r1         = GPIO_BIT(12),
    .p1_g1         = GPIO_BIT(5),
    .p1_b1         = GPIO_BIT(6),
    .p1_r2         = GPIO_BIT(19),
    .p1_g2         = GPIO_BIT(13),
    .p1_b2         = GPIO_BIT(20),

    /* Chain 2 */
    .p2_r1         = GPIO_BIT(14), /* masks TxD when parallel=3 */
    .p2_g1         = GPIO_BIT(2),  /* masks SCL when parallel=3 */
    .p2_b1         = GPIO_BIT(3),  /* masks SDA when parallel=3 */
    .p2_r2         = GPIO_BIT(26),
    .p2_g2         = GPIO_BIT(16),
    .p2_b2         = GPIO_BIT(21),
  },
```

## Scoreboard Matrix C++ Configuration Code
```cpp
matrix_options.hardware_mapping = "regular";  // or e.g. "adafruit-hat"
matrix_options.pixel_mapper_config = "Rotate:180;264-Mapper"; // or e.g. "U-mapper" 
matrix_options.rows = 64;
matrix_options.cols = 64;
matrix_options.chain_length = 1;
matrix_options.parallel = 2;
matrix_options.show_refresh_rate = false;
matrix_options.disable_hardware_pulsing = true; // --led-no-hardware-pulse
```

# rgb_matrix
Here is the source code for the C++ header file of the GPIO C++ controller code that we are using:
```cpp
// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Copyright (C) 2013 Henner Zeller <h.zeller@acm.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://gnu.org/licenses/gpl-2.0.txt>

#ifndef RPI_GPIO_INTERNAL_H
#define RPI_GPIO_INTERNAL_H

#include "gpio-bits.h"

#include <vector>

// Putting this in our namespace to not collide with other things called like
// this.
namespace rgb_matrix {
// For now, everything is initialized as output.
class GPIO {
public:
  GPIO();

  // Initialize before use. Returns 'true' if successful, 'false' otherwise
  // (e.g. due to a permission problem).
  bool Init(int slowdown);

  // Initialize outputs.
  // Returns the bits that were available and could be set for output.
  // (never use the optional adafruit_hack_needed parameter, it is used
  // internally to this library).
  gpio_bits_t InitOutputs(gpio_bits_t outputs,
                          bool adafruit_hack_needed = false);

  // Request given bitmap of GPIO inputs.
  // Returns the bits that were available and could be reserved.
  gpio_bits_t RequestInputs(gpio_bits_t inputs);

  // Set the bits that are '1' in the output. Leave the rest untouched.
  inline void SetBits(gpio_bits_t value) {
    if (!value) return;
    WriteSetBits(value);
    for (int i = 0; i < slowdown_; ++i) {
      WriteSetBits(value);
    }
  }

  // Clear the bits that are '1' in the output. Leave the rest untouched.
  inline void ClearBits(gpio_bits_t value) {
    if (!value) return;
    WriteClrBits(value);
    for (int i = 0; i < slowdown_; ++i) {
      WriteClrBits(value);
    }
  }

  // Write all the bits of "value" mentioned in "mask". Leave the rest untouched.
  inline void WriteMaskedBits(gpio_bits_t value, gpio_bits_t mask) {
    // Writing a word is two operations. The IO is actually pretty slow, so
    // this should probably  be unnoticable.
    ClearBits(~value & mask);
    SetBits(value & mask);
  }

  inline gpio_bits_t Read() const { return ReadRegisters() & input_bits_; }

  // Return if this is appears to be a Pi4
  static bool IsPi4();

private:
  inline gpio_bits_t ReadRegisters() const {
    return (static_cast<gpio_bits_t>(*gpio_read_bits_low_)
#ifdef ENABLE_WIDE_GPIO_COMPUTE_MODULE
            | (static_cast<gpio_bits_t>(*gpio_read_bits_low_) << 32)
#endif
            );
  }

  inline void WriteSetBits(gpio_bits_t value) {
    *gpio_set_bits_low_ = static_cast<uint32_t>(value & 0xFFFFFFFF);
#ifdef ENABLE_WIDE_GPIO_COMPUTE_MODULE
    if (uses_64_bit_)
      *gpio_set_bits_high_ = static_cast<uint32_t>(value >> 32);
#endif
  }

  inline void WriteClrBits(gpio_bits_t value) {
    *gpio_clr_bits_low_ = static_cast<uint32_t>(value & 0xFFFFFFFF);
#ifdef ENABLE_WIDE_GPIO_COMPUTE_MODULE
    if (uses_64_bit_)
      *gpio_clr_bits_high_ = static_cast<uint32_t>(value >> 32);
#endif
  }

private:
  gpio_bits_t output_bits_;
  gpio_bits_t input_bits_;
  gpio_bits_t reserved_bits_;
  int slowdown_;

  volatile uint32_t *gpio_set_bits_low_;
  volatile uint32_t *gpio_clr_bits_low_;
  volatile uint32_t *gpio_read_bits_low_;

#ifdef ENABLE_WIDE_GPIO_COMPUTE_MODULE
  bool uses_64_bit_;
  volatile uint32_t *gpio_set_bits_high_;
  volatile uint32_t *gpio_clr_bits_high_;
  volatile uint32_t *gpio_read_bits_high_;
#endif
};

// A PinPulser is a utility class that pulses a GPIO pin. There can be various
// implementations.
class PinPulser {
public:
  // Factory for a PinPulser. Chooses the right implementation depending
  // on the context (CPU and which pins are affected).
  // "gpio_mask" is the mask that should be output (since we only
  //   need negative pulses, this is what it does)
  // "nano_wait_spec" contains a list of time periods we'd like
  //   invoke later. This can be used to pre-process timings if needed.
  static PinPulser *Create(GPIO *io, gpio_bits_t gpio_mask,
                           bool allow_hardware_pulsing,
                           const std::vector<int> &nano_wait_spec);

  virtual ~PinPulser() {}

  // Send a pulse with a given length (index into nano_wait_spec array).
  virtual void SendPulse(int time_spec_number) = 0;

  // If SendPulse() is asynchronously implemented, wait for pulse to finish.
  virtual void WaitPulseFinished() {}
};

// Get rolling over microsecond counter. We get this from a hardware register
// if possible and a terrible slow fallback otherwise.
uint32_t GetMicrosecondCounter();

}  // end namespace rgb_matrix

#endif  // RPI_GPIO_INGERNALH
```

# GPIO and PulseTimer implementations

```cpp
bool GPIO::Init(int slowdown) {
  slowdown_ = slowdown;

  // Pre-mmap all bcm registers we need now and possibly in the future, as to
  // allow  dropping privileges after GPIO::Init() even as some of these
  // registers might be needed later.
  if (!mmap_all_bcm_registers_once())
    return false;

  gpio_set_bits_low_ = s_GPIO_registers + (0x1C / sizeof(uint32_t));
  gpio_clr_bits_low_ = s_GPIO_registers + (0x28 / sizeof(uint32_t));
  gpio_read_bits_low_ = s_GPIO_registers + (0x34 / sizeof(uint32_t));

#ifdef ENABLE_WIDE_GPIO_COMPUTE_MODULE
  gpio_set_bits_high_ = s_GPIO_registers + (0x20 / sizeof(uint32_t));
  gpio_clr_bits_high_ = s_GPIO_registers + (0x2C / sizeof(uint32_t));
  gpio_read_bits_high_ = s_GPIO_registers + (0x38 / sizeof(uint32_t));
#endif

  return true;
}

GPIO::GPIO() : output_bits_(0), input_bits_(0), reserved_bits_(0),
               slowdown_(1)
#ifdef ENABLE_WIDE_GPIO_COMPUTE_MODULE
             , uses_64_bit_(false)
#endif
{
}

gpio_bits_t GPIO::RequestInputs(gpio_bits_t inputs) {
  if (s_GPIO_registers == NULL) {
    fprintf(stderr, "Attempt to init inputs but not yet Init()-ialized.\n");
    return 0;
  }

  inputs &= ~(output_bits_ | input_bits_ | reserved_bits_);
#ifdef ENABLE_WIDE_GPIO_COMPUTE_MODULE
  const int kMaxAvailableBit = 45;
  uses_64_bit_ |= (inputs >> 32) != 0;
#else
  const int kMaxAvailableBit = 31;
#endif
  for (int b = 0; b <= kMaxAvailableBit; ++b) {
    if (inputs & GPIO_BIT(b)) {
      INP_GPIO(b);
    }
  }
  input_bits_ |= inputs;
  return inputs;
}

// Public PinPulser factory
PinPulser *PinPulser::Create(GPIO *io, gpio_bits_t gpio_mask,
                             bool allow_hardware_pulsing,
                             const std::vector<int> &nano_wait_spec) {
  if (!Timers::Init()) return NULL;
  if (allow_hardware_pulsing && HardwarePinPulser::CanHandle(gpio_mask)) {
    return new HardwarePinPulser(gpio_mask, nano_wait_spec);
  } else {
    return new TimerBasedPinPulser(io, gpio_mask, nano_wait_spec);
  }
}

// Simplest of PinPulsers. Uses somewhat jittery and manual timers
// to get the timing, but not optimal.
class TimerBasedPinPulser : public PinPulser {
public:
  TimerBasedPinPulser(GPIO *io, gpio_bits_t bits,
                      const std::vector<int> &nano_specs)
    : io_(io), bits_(bits), nano_specs_(nano_specs) {
    if (!s_Timer1Mhz) {
      fprintf(stderr, "FYI: not running as root which means we can't properly "
              "control timing unless this is a real-time kernel. Expect color "
              "degradation. Consider running as root with sudo.\n");
    }
  }

  virtual void SendPulse(int time_spec_number) {
    io_->ClearBits(bits_);
    Timers::sleep_nanos(nano_specs_[time_spec_number]);
    io_->SetBits(bits_);
  }

private:
  GPIO *const io_;
  const gpio_bits_t bits_;
  const std::vector<int> nano_specs_;
};
// A PinPulser that uses the PWM hardware to create accurate pulses.
// It only works on GPIO-12 or 18 though.
class HardwarePinPulser : public PinPulser {
public:
  static bool CanHandle(gpio_bits_t gpio_mask) {
#ifdef DISABLE_HARDWARE_PULSES
    return false;
#else
    const bool can_handle = gpio_mask==GPIO_BIT(18) || gpio_mask==GPIO_BIT(12);
    if (can_handle && (s_PWM_registers == NULL || s_CLK_registers == NULL)) {
      // Instead of silently not using the hardware pin pulser and falling back
      // to timing based loops, complain loudly and request the user to make
      // a choice before continuing.
      fprintf(stderr, "Need root. You are configured to use the hardware pulse "
              "generator "
              "for\n\tsmooth color rendering, however the necessary hardware\n"
              "\tregisters can't be accessed because you probably don't run\n"
              "\twith root permissions or privileges have been dropped.\n"
              "\tSo you either have to run as root (e.g. using sudo) or\n"
              "\tsupply the --led-no-hardware-pulse command-line flag.\n\n"
              "\tExiting; run as root or with --led-no-hardware-pulse\n\n");
      exit(1);
    }
    return can_handle;
#endif
  }

  HardwarePinPulser(gpio_bits_t pins, const std::vector<int> &specs)
    : triggered_(false) {
    assert(CanHandle(pins));
    assert(s_CLK_registers && s_PWM_registers && s_Timer1Mhz);

#if DEBUG_SLEEP_JITTER
    atexit(print_overshoot_histogram);
#endif

    if (LinuxHasModuleLoaded("snd_bcm2835")) {
      fprintf(stderr,
              "\n%s=== snd_bcm2835: found that the Pi sound module is loaded. ===%s\n"
              "Don't use the built-in sound of the Pi together with this lib; it is known to be\n"
	      "incompatible and cause trouble and hangs (you can still use external USB sound adapters).\n\n"
              "See Troubleshooting section in README how to disable the sound module.\n"
	      "You can also run with --led-no-hardware-pulse to avoid the incompatibility,\n"
	      "but you will have more flicker.\n"
              "Exiting; fix the above first or use --led-no-hardware-pulse\n\n",
              "\033[1;31m", "\033[0m");
      exit(1);
    }

    for (size_t i = 0; i < specs.size(); ++i) {
      // Hints how long to nanosleep, already corrected for system overhead.
      sleep_hints_us_.push_back(specs[i]/1000 - JitterAllowanceMicroseconds());
    }

    const int base = specs[0];
    // Get relevant registers
    fifo_ = s_PWM_registers + PWM_FIFO;

    if (pins == GPIO_BIT(18)) {
      // set GPIO 18 to PWM0 mode (Alternative 5)
      SetGPIOMode(s_GPIO_registers, 18, 2);
    } else if (pins == GPIO_BIT(12)) {
      // set GPIO 12 to PWM0 mode (Alternative 0)
      SetGPIOMode(s_GPIO_registers, 12, 4);
    } else {
      assert(false); // should've been caught by CanHandle()
    }
    InitPWMDivider((base/2) / PWM_BASE_TIME_NS);
    for (size_t i = 0; i < specs.size(); ++i) {
      pwm_range_.push_back(2 * specs[i] / base);
    }
  }

  virtual void SendPulse(int c) {
    if (pwm_range_[c] < 16) {
      s_PWM_registers[PWM_RNG1] = pwm_range_[c];

      *fifo_ = pwm_range_[c];
    } else {
      // Keep the actual range as short as possible, as we have to
      // wait for one full period of these in the zero phase.
      // The hardware can't deal with values < 2, so only do this when
      // have enough of these.
      s_PWM_registers[PWM_RNG1] = pwm_range_[c] / 8;

      *fifo_ = pwm_range_[c] / 8;
      *fifo_ = pwm_range_[c] / 8;
      *fifo_ = pwm_range_[c] / 8;
      *fifo_ = pwm_range_[c] / 8;
      *fifo_ = pwm_range_[c] / 8;
      *fifo_ = pwm_range_[c] / 8;
      *fifo_ = pwm_range_[c] / 8;
      *fifo_ = pwm_range_[c] / 8;
    }

    /*
     * We need one value at the end to have it go back to
     * default state (otherwise it just repeats the last
     * value, so will be constantly 'on').
     */
    *fifo_ = 0;   // sentinel.

    /*
     * For some reason, we need a second empty sentinel in the
     * fifo, otherwise our way to detect the end of the pulse,
     * which relies on 'is the queue empty' does not work. It is
     * not entirely clear why that is from the datasheet,
     * but probably there is some buffering register in which data
     * elements are kept after the fifo is emptied.
     */
    *fifo_ = 0;

    sleep_hint_us_ = sleep_hints_us_[c];
    start_time_ = *s_Timer1Mhz;
    triggered_ = true;
    s_PWM_registers[PWM_CTL] = PWM_CTL_USEF1 | PWM_CTL_PWEN1 | PWM_CTL_POLA1;
  }

  virtual void WaitPulseFinished() {
    if (!triggered_) return;
    // Determine how long we already spent and sleep to get close to the
    // actual end-time of our sleep period.
    //
    // TODO(hzeller): find if it is possible to get some sort of interrupt from
    //   the hardware once it is done with the pulse. Sounds silly that there is
    //   not (so far, only tested GPIO interrupt with a feedback line, but that
    //   is super-slow with 20μs overhead).
    if (sleep_hint_us_ > 0) {
      const uint32_t already_elapsed_usec = *s_Timer1Mhz - start_time_;
      const int to_sleep_us = sleep_hint_us_ - already_elapsed_usec;
      if (to_sleep_us > 0) {
        struct timespec sleep_time = { 0, 1000 * to_sleep_us };
        nanosleep(&sleep_time, NULL);

#if DEBUG_SLEEP_JITTER
        {
          // Record histogram of realtime jitter how much longer we actually
          // took.
          const int total_us = *s_Timer1Mhz - start_time_;
          const int nanoslept_us = total_us - already_elapsed_usec;
          int overshoot = nanoslept_us - (to_sleep_us + JitterAllowanceMicroseconds());
          if (overshoot < 0) overshoot = 0;
          if (overshoot > 255) overshoot = 255;
          overshoot_histogram_us[overshoot]++;
        }
#endif
      }
    }

    while ((s_PWM_registers[PWM_STA] & PWM_STA_EMPT1) == 0) {
      // busy wait until done.
    }
    s_PWM_registers[PWM_CTL] = PWM_CTL_USEF1 | PWM_CTL_POLA1 | PWM_CTL_CLRF1;
    triggered_ = false;
  }

private:
  void SetGPIOMode(volatile uint32_t *gpioReg, unsigned gpio, unsigned mode) {
    const int reg = gpio / 10;
    const int mode_pos = (gpio % 10) * 3;
    gpioReg[reg] = (gpioReg[reg] & ~(7 << mode_pos)) | (mode << mode_pos);
  }

  void InitPWMDivider(uint32_t divider) {
    assert(divider < (1<<12));  // we only have 12 bits.

    s_PWM_registers[PWM_CTL] = PWM_CTL_USEF1 | PWM_CTL_POLA1 | PWM_CTL_CLRF1;

    // reset PWM clock
    s_CLK_registers[CLK_PWMCTL] = CLK_PASSWD | CLK_CTL_KILL;

    // set PWM clock source as 500 MHz PLLD
    s_CLK_registers[CLK_PWMCTL] = CLK_PASSWD | CLK_CTL_SRC(CLK_CTL_SRC_PLLD);

    // set PWM clock divider
    s_CLK_registers[CLK_PWMDIV]
      = CLK_PASSWD | CLK_DIV_DIVI(divider) | CLK_DIV_DIVF(0);

    // enable PWM clock
    s_CLK_registers[CLK_PWMCTL]
      = CLK_PASSWD | CLK_CTL_ENAB | CLK_CTL_SRC(CLK_CTL_SRC_PLLD);
  }

private:
  std::vector<uint32_t> pwm_range_;
  std::vector<int> sleep_hints_us_;
  volatile uint32_t *fifo_;
  uint32_t start_time_;
  int sleep_hint_us_;
  bool triggered_;
};
```

# Your Task
We need 4 lines to use for our remote system.  I need a C++ utility solution that with set up 4 GPIO pins for inputs and continually read these pins.  First we will just print the 4 values to stdout.
