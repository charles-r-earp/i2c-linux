#ifndef I2C_LINUX_PWM_DRIVER_HPP
#define I2C_LINUX_PWM_DRIVER_HPP


#include <i2c_device.hpp>

#include <cmath>
#include <thread>
#include <chrono> 

namespace i2c_linux {
    
    // Adafruit P815
    
    struct pwm_driver : i2c_device {
        
        // Registers/etc:
        const int PCA9685_ADDRESS = 0x40;
        const int MODE1 = 0x00;
        const int MODE2 = 0x01;
        const int SUBADR1 = 0x02;
        const int SUBADR2 = 0x03;
        const int SUBADR3 = 0x04;
        const int PRESCALE = 0xFE;
        const int LED0_ON_L = 0x06;
        const int LED0_ON_H = 0x07;
        const int LED0_OFF_L = 0x08;
        const int LED0_OFF_H = 0x09;
        const int ALL_LED_ON_L = 0xFA;
        const int ALL_LED_ON_H = 0xFB;
        const int ALL_LED_OFF_L = 0xFC;
        const int ALL_LED_OFF_H = 0xFD;
        
        // Bits:
        const int RESTART = 0x80;
        const int SLEEP = 0x10;
        const int ALLCALL = 0x01;
        const int INVRT = 0x10;
        const int OUTDRV = 0x04;
        
        struct pwm_device {
            
            int channel;
            
            pwm_device(int channel = 0) {
            
                this->channel = channel;
            }
        };
        
        struct servo : pwm_device {
            
            using pwm_device::pwm_device;
            
            mutable int min_pw_ms = 150;
            mutable int max_pw_ms = 600;
        };
        
        struct bi_directonal_motor : pwm_device {
            
            int direction_channel;
            
            bi_directional_motor(int channel = 0, direction_channel = channel + 1) : pwm_device(channel) {
                
                this->direction_channel = direction_channel;
            }
            
        };
        
        
        pwm_driver(const int address = 0x40, double freq_hz = 60) : i2c_device(address) {
        
            this->set_all_pwm(0, 0);
            this->write8(MODE2, OUTDRV);
            this->write8(MODE1, ALLCALL);
            
            std::this_thread::sleep_for (std::chrono::milliseconds(5));
            
            auto mode1 = this->read_U8(MODE1);
            mode1 = mode1 & ~SLEEP;
            this->write8(MODE1, mode1);
            
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            
            this->set_pwm_freq(freq_hz);
        }
        
        void set_pwm_freq(double freq_hz) {
            
            //"""Set the PWM frequency to the provided value in hertz."""
            double prescaleval = 25000000.0;    // 25MHz
            prescaleval /= 4096.0;     // 12-bit
            prescaleval /= freq_hz;
            prescaleval -= 1.0;
        //logger.debug('Setting PWM frequency to {0} Hz'.format(freq_hz))
            std::cout << "Setting PWM frequency to " << freq_hz <<  " Hz" << std::endl; 
        //logger.debug('Estimated pre-scale: {0}'.format(prescaleval))
            std::cout << "Estimated pre-scale: " << prescaleval << std::endl; 
            int prescale = int(std::floor(prescaleval + 0.5));
            //logger.debug('Final pre-scale: {0}'.format(prescale))
            std::cout << "Final pre-scale: " << prescale << std::endl;
            int oldmode = this->read_U8(MODE1);
            int newmode = (oldmode & 0x7F) | 0x10;    // sleep
            this->write8(MODE1, newmode);  // go to sleep
            this->write8(PRESCALE, prescale);
            this->write8(MODE1, oldmode);
                
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            
            this->write8(MODE1, oldmode | 0x80);
        }
        
        void set_pwm(int channel, int on, int off) {
            
            std::cout << "set_pwm(" << channel << ", " << on << ", " << off << ")" << std::endl;
            
            //"""Sets a single PWM channel."""
            this->write8(LED0_ON_L+4*channel, on & 0xFF);
            this->write8(LED0_ON_H+4*channel, on >> 8);
            this->write8(LED0_OFF_L+4*channel, off & 0xFF);
            this->write8(LED0_OFF_H+4*channel, off >> 8);
        }

        void set_all_pwm(int on, int off) {
            //"""Sets all PWM channels."""
            this->write8(ALL_LED_ON_L, on & 0xFF);
            this->write8(ALL_LED_ON_H, on >> 8);
            this->write8(ALL_LED_OFF_L, off & 0xFF);
            this->write8(ALL_LED_OFF_H, off >> 8);
        }
        
/*
        // Helper function to make setting a servo pulse width simpler.
        void set_servo_pulse(int channel, int pulse) {
            
            int pulse_length = 1000000; //   # 1,000,000 us per second
            pulse_length /= 60;     //  # 60 Hz
            //print('{0}us per period'.format(pulse_length))
            pulse_length /= 4096;  //   # 12 bits of resolution
            //print('{0}us per bit'.format(pulse_length))
            pulse *= 1000;
            pulse /= pulse_length;
            this->set_pwm(channel, 0, pulse);
        }*/
        
        void set_ratio(const servo& s, double ratio) {
            // ratio from 0 to 1
        
            this->set_pwm(s.channel, 0, ratio*(s.max_pw_ms-s.min_pw_ms) + s.min_pw_ms);
        }
        
        void set_speed_ratio(const bi_directional_motor& motor, const double& speed_ratio) {
            // speed_ratio from -1 to 1
            
            int pulse_length = 4096*std::abs(speed_ratio);
            int direction = speed_ration ? speed_ratio >= 0 ? 1 : -1;
            
            this->set_pwm(motor.channel, pulse_length, 0);
            this->set_pwm(motor.direction_channel, 4096, 0);
        }
        
    };
}



#endif // I2C_LINUX_PWM_DRIVER_HPP
