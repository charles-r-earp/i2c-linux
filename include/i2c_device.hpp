#ifndef I2C_LINUX_I2C_DEVICE_HPP
#define I2C_LINUX_I2C_DEVICE_HPP

#include "i2c-dev.h"
#include "smbus.h"

#include <cstddef> // define NULL
#include <sys/ioctl.h>
#include <fcntl.h>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>

namespace i2c_linux {

    std::string decimal_to_hex(int decimal_value) {
        
        std::stringstream ss;
        ss << std::hex << decimal_value; // int decimal_value
        
        return std::string( ss.str() );
    }
    
    struct i2c_device {
        
        int address;
        int file; 
        
        mutable bool debug = false;
        
        i2c_device(const int& address, const std::string device = "/dev/i2c-1") {
            
            file = open(device.c_str(), O_RDWR);
            
            if (file < 0) {
                
                std::cout << "i2c_device() error = " << errno << std::endl;
                
                abort();
            }
            
            this->address = address;
        }
        
        bool check_address() {
            
            if (ioctl(this->file, I2C_SLAVE, this->address) < 0) {
                
                if (this->debug) std::cout << "i2c_device.check_address address = " << address << " error = " << file << std::endl;
                
                return false;
            }
            else {
                
                return true;
            }
            
        }
        
        void write_raw8(const int& value) {
            
            assert(this->check_address());
            
            i2c_smbus_write_byte(this->file, value);
        }
        
        void write8(const int& reg, const int& value) {
            
            assert(this->check_address());
            
            i2c_smbus_write_byte_data(this->file, reg, value);
            
            if (this->debug) std::cout << "Wrote " << decimal_to_hex(value) << " to register: " << decimal_to_hex(reg) << std::endl;
        }
        
        void write16(const int& reg, const int& value) {
            
            assert(this->check_address());
            
            i2c_smbus_write_word_data(this->file, reg, value);
            
            if (this->debug) std::cout << "Wrote " << decimal_to_hex(value) << " to register: " << decimal_to_hex(reg) << std::endl;
        }
        
        int read_raw8(const int& reg) {
            
            assert(this->check_address());
            
            //this->write(reg);
            
            int value = i2c_smbus_read_byte(file);
            
            if (this->debug) std::cout << "Read " << decimal_to_hex(value) << " from register: " << decimal_to_hex(reg) << std::endl;
            
            return value;
        }
        
        int read_U8(const int& reg) {
            
            assert(this->check_address());
            
            int value = i2c_smbus_read_byte_data(file, reg);
            
            if (this->debug) std::cout << "Read " << decimal_to_hex(value) << " from register: " << decimal_to_hex(reg) << std::endl;
            
            return value;
        }
        
    };
}


#endif // I2C_LINUX_I2C_DEVICE_HPP
