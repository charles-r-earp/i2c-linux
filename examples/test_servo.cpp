#include <i2c-linux/pwm_driver.hpp>  

int main() {
    
    i2c_linux::pwm_driver pwm_driver; // defaults to adress of 0x40 and 60 hz
    
    i2c_linux::pwm_driver::servo servo(0); // Channel 0
    
    while (true) {
        
        pwm_driver.set_ratio(servo, 0);
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        pwm_driver.set_ratio(servo, 0.5);
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        pwm_driver.set_ratio(servo, 1);
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        pwm_driver.set_ratio(servo, 0.5);
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}
