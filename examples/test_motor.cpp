#include <pwm_driver.hpp>  

int main() {
    
    i2c_linux::pwm_driver pwm_driver; // defaults to adress of 0x40 and 60 hz
    
    i2c_linux::pwm_driver::bi_directional_motor motor(4, 5); // Channels 4 and 5
    
    while (true) {
        
        for (double speed : {0.0, 0.5, 1.0, 0.0, -0.5, -1.0}) {
            
            pwm_driver.set_speed_ratio(motor, speed);
        
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    
    return 0;
}