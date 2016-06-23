#include "mbed.h"
#include "Blue_ESC.h"

/* I2C buffer setup for the ESC
Byte 0: pulse_count_h
Byte 1: pulse_count_l
Byte 2: voltage_h
Byte 3: voltage_l
Byte 4: temperature_h
Byte 5: temperature_l
Byte 6: current_h
Byte 7: current_l
Byte 8: 0xab (identifier to check if ESC is alive)
*/

namespace {
    uint8_t _buffer[9];
    I2C i2c(I2C_SDA, I2C_SCL); //these pins will probably have to be changed
    Timer _mytimer;
}

    void Blue_ESC::update() {
        _buffer[8] = 0x00;
        
        readBuffer(_address, _buffer);
        
        _rpm = convertBytesToInt(_buffer[0], _buffer[1]);
        _voltage_raw = convertBytesToInt(_buffer[2], _buffer[3]);
        _temp_raw = convertBytesToInt(_buffer[4], _buffer[5]);
        _current_raw = convertBytesToInt(_buffer[6], _buffer[7]);
        _identifier = _buffer[8];
        
        _rpm = float(_rpm)/((uint16_t(_mytimer.read_ms())-_rpmTimer)/1000.0f)*60/float(_poleCount);
        _rpmTimer = uint16_t(_mytimer.read_ms());
   }
    
    //convert two bytes into an 16 bit integer
    int16_t Blue_ESC::convertBytesToInt(uint8_t byte_h, uint8_t byte_l){
        int16_t result = (byte_h << 8) | byte_l;
        return result;
    }
    
    void Blue_ESC::readBuffer(uint8_t address, uint8_t buffer[]){
        
        i2c.write(0x02); //data start register
        wait(1.0/1000000.0);
        char _tmp_buffer[9];
        if (i2c.read(address, _tmp_buffer, sizeof(_tmp_buffer), false) == ACK) {
            //good
        } else {
            //bad
            throw ERR_I2C_READ_FAIL;
        }
        
        for(int i = 0; i<9; i++)
            _buffer[i] = _tmp_buffer[i];
            
    }
    
    void Blue_ESC::set(float percent) {
        //should be a value between -1.0 and 1.0
        if (percent > 1.0f || percent < -1.0f) {
            //unacceptable value - maybe throw an exception instead of doing nothing?
            throw ERR_INVALID_RANGE;
            return;
        }
        
        //32767 is max forward, -32767 max reverse
        int16_t rawThrottle = floor(percent * 32767);
        this.setRaw(rawThrottle);
    }

    void Blue_ESC::setRaw(int16_t rawThrottle) {
        //just want the high bits
        uint8_t throttle_h = rawThrottle >> 8;
        //just need the last 8 bits
        uint8_t throttle_l = rawThrottle;
    
        char data[2];
        data[0] = throttle_h;
        data[1] = throttle_l;
    
        if (i2c.write(0x00) != ACK) {
            throw ERR_I2C_WRITE_FAIL;
        }
        if (i2c.write(_address, data, sizeof(data), false) != ACK) {
            throw ERR_I2C_WRITE_FAIL;
        }
    }

    Blue_ESC::Blue_ESC(uint8_t address, uint8_t poleCount){
        _address = address;
        _poleCount = poleCount;
        _mytimer.start();
        
        i2c.frequency(100); //in hz
        
        //must send 0 to motor to initialize it or nothing will happen
        setRaw(0);
    }
    
    float Blue_ESC::voltage(){
        return float(_voltage_raw)/65536.0f*5.0f*6.45f;
    }
    
    float Blue_ESC::temperature(){
        //code from Arduino
        float resistance = SERIESRESISTOR/(65535/float(_temp_raw)-1);

        float steinhart;
        steinhart = resistance / THERMISTORNOMINAL;  // (R/Ro)
        steinhart = log(steinhart);                  // ln(R/Ro)
        steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
        steinhart += 1.0f / (TEMPERATURENOMINAL + 273.15f); // + (1/To)
        steinhart = 1.0f / steinhart;                 // Invert
        steinhart -= 273.15f;                         // convert to C
        return steinhart;
    }
    
    bool Blue_ESC::isAlive(){
        return (_identifier == 0xab);
    }
    
    int16_t Blue_ESC::rpm(){
        return _rpm;
    }
  
    float Blue_ESC::current() {
        return (float(_current_raw)-32767)/65535.0f*5.0f*14.706f;
    }
    

    