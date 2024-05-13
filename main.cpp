#include "mbed.h"
#include "uop_msb.h"

#define TRAFFIC_LIGHT_DELAY 1000
#define ENV_SAMPLE_DELAY 500

// Function prototypes
void Traffic_Lights();
void LCD_BackLight_Effect();
void Bar_Flash();
void clearMatrix();
void matrix_scan();
void sevenseg_count_thread();
float sample_adc(int);
void environment_data();
void dac_thread();
void dac_out(int, float);
void mpu_thread();
void button_thread();

// Thread handles
Thread traffic_thr;
Thread lcd_bl_thr;
Thread bar_thr;
Thread matrix_thr;
Thread seven_seg_thr;
Thread env_sensor_thr;
Thread dac_thr;
Thread motion_thr;
Thread button_thr;

using namespace uop_msb;

int main()
{
    printf("\n\nStarting...\n");

    /*********SD Card*********/
    
    // Create test string 
    char test_string[] = "This is the Plymouth University EEER Module Support Board Test\n";
    // Write the string to a file
    sd.write_file("msb_test.txt", test_string,true);
    // print the contents of the file we've just written to
    sd.print_file("msb_test.txt",true);
    // Copy the contents back into another array
    char array_to_copy_to[256];
    sd.copy_file("msb_test.txt", array_to_copy_to,sizeof(array_to_copy_to),true);
    // Print the new array
    printf("%s\n",array_to_copy_to);

    // Clear the seven segment display
    latchedLEDs.sevenSegclear();
    // Switcvh off the pedestrian LED
    traffic.set(TRAF_PED,0);

    // Display welcome messages
    printf("Starting Program..\n");
    disp.cls();
    disp.home();
    disp.printf("   SECaM PU");
    disp.locate(1,0);
    disp.printf("   MSB Test");
    ThisThread::sleep_for(2000ms);
    disp.cls();

    // Play a short tone on the buzzer
    buzz.playTone("C", Buzzer::MIDDLE_OCTAVE);
    ThisThread::sleep_for(200ms);
    buzz.rest();

    // Start all the threads!
    traffic_thr.start(Traffic_Lights);
    lcd_bl_thr.start(LCD_BackLight_Effect);
    bar_thr.start(Bar_Flash);
    matrix_thr.start(matrix_scan);
    seven_seg_thr.start(sevenseg_count_thread);
    env_sensor_thr.start(environment_data);
    dac_thr.start(dac_thread);
    motion_thr.start(mpu_thread);
    button_thr.start(button_thread);


    while(1){
        ThisThread::sleep_for(osWaitForever);
    }
}

// Function: button_thread (Thread)
// This function exercises the A,B,C and D button on the right of the board.
// When a button is pressed, its name will be displayed on the LCD along with
// and ADC reading. By pressing different buttons, different ADC channels are sampled
void button_thread(){

    /********* Buttons *********/

   while(true){
        char switchNum=' ';
        // Check for button press  
        if (buttons.Button1.read())switchNum='A';
        if (buttons.Button2.read())switchNum='B';
        if (buttons.Button3.read())switchNum='C';
        if (buttons.Button4.read())switchNum='D';

        // Notify of button press on LCD and play a tone
        if(switchNum){
            disp.cls();                                 // clear display
            disp.locate(0,1);                           
            disp.printf("  Switch = %c",switchNum);     // write the switch name
            char sw[1];
            sw[0] = switchNum;                          // Play a note which corresponds to the
            ThisThread::sleep_for(20ms);                // switch name (Middle Octave)
            buzz.playTone(sw,Buzzer::MIDDLE_OCTAVE);
        }
        // Sample the associated ADC Channel
        switch(switchNum){
            case 'A': disp.locate(1,0);disp.printf("  SIG IN %4.2fV",sample_adc(0));break;
            case 'B': disp.locate(1,0);disp.printf("  POT IN %4.2fV",sample_adc(1));break;
            case 'C': disp.locate(1,0);disp.printf("  LDR IN %4.2fV",sample_adc(2));break;
            case 'D': disp.locate(1,0);disp.printf("  MIC IN %4.2fV",sample_adc(3));break;
            default:disp.cls();disp.locate(0,1);disp.printf(" Press Switch");break;
        }
        // Stop the buzzer after a short time
        ThisThread::sleep_for(200ms);
        buzz.rest();
        ThisThread::sleep_for(100ms);
    }

}

// Function: mpu_thread (Thread)
// Periodically requests inertial measurmement data from the MPU6050
// accelerometer and gyroscope. The data is then displayed on the terminal
void mpu_thread(){

    /********** Motion Sensor *********/

    while(1){
        Motion_t acc = motion.getAcceleration();
        Motion_t gyro = motion.getGyro();
        printf("Gyro x:%3.3f Gyro y:%3.3f Gyro z:%3.3f\n", gyro.x,gyro.y,gyro.z);
        printf("Acc x:%3.3f Acc y:%3.3f Acc z:%3.3f\n", acc.x,acc.y,acc.z);
        ThisThread::sleep_for(500ms);
    }
}

#define SAMPLES 10
// Function: sample_adc
// Samples an ADC channel seleced by passing the ADsource value
// A number of samples are taken and then averaged
// Converts the average to a voltage and returns as a float 

float sample_adc(int ADsource){

    /********** Analog-to_Digital Converters *********/

    unsigned int adc_sample[SAMPLES+1]; // was unsigned int
    float Voltage;
    adc_sample[0]=0;    // we use index 0 for the averaging
    for(int i=1; i<=SAMPLES; i++){
        switch(ADsource){
            case 0: adc_sample[i] = signal_in.read_u16();break;
            case 1: adc_sample[i] = pot.read_u16();break;
            case 2: adc_sample[i] = ldr.read_u16();break;
            case 3: adc_sample[i] = mic.read_u16();break;
            default: break;
        }

        adc_sample[0]+=adc_sample[i];
        ThisThread::sleep_for(10ms);
    }
    Voltage = 3.3f * ((float)adc_sample[0]/(float)SAMPLES)/65535.0f;
    return Voltage;
}


// Function: LCD_BackLight_Effect (Thread)
// This produces a fading effect on the LCD backlight
// This may not work with all versions of the LCD display
void LCD_BackLight_Effect(){

    /********** LCD Backlight *********/

    for(float i=0; i<1.0f; i+=0.01){
        disp.backlight_brightness(i);
        ThisThread::sleep_for(20ms);
    }
}


// Function: Traffic_Lights (Thread)
// Displays a traffic light pattern on the traffic light LEDs
void Traffic_Lights(){

    /********** Traffic Lights *********/

    while (true)
    {
        traffic.set(TRAF_PED,1);
        traffic.set(TRAF_SET_1,1);
        traffic.set(TRAF_SET_2,3);
        ThisThread::sleep_for(TRAFFIC_LIGHT_DELAY);

        traffic.set(TRAF_PED,0);
        traffic.set(TRAF_SET_1,3);
        traffic.set(TRAF_SET_2,5);
        ThisThread::sleep_for(TRAFFIC_LIGHT_DELAY);

        traffic.set(TRAF_SET_1,4);
        traffic.set(TRAF_SET_2,6);
        ThisThread::sleep_for(TRAFFIC_LIGHT_DELAY);

        traffic.set(TRAF_SET_1,2);
        traffic.set(TRAF_SET_2,4);
        ThisThread::sleep_for(TRAFFIC_LIGHT_DELAY);
    }
}

// Function: Bar_Flash (Thread)
// Displays a number of test patterns on the RGB LED strips
void Bar_Flash()
{
    /********** RGB Strips (Latched LEDS) *********/

    latchedLEDs.enable(true);
    while(true)
    {
        // Flash all LEDS on the 3 strips
        for(int i=0;i<8;i++){
            latchedLEDs.write_strip(255*(i%2),LEDGROUP::RED);
            latchedLEDs.write_strip(255*(i%2),LEDGROUP::GREEN);
            latchedLEDs.write_strip(255*(i%2),LEDGROUP::BLUE);
            ThisThread::sleep_for(200ms);
        }
        // Accend through all 3 strips at once
        for(int q=0;q<6;q++){
            for(int i=1;i<256;i*=2){
                latchedLEDs.write_strip(i,LEDGROUP::RED);
                latchedLEDs.write_strip(i,LEDGROUP::GREEN);
                latchedLEDs.write_strip(i,LEDGROUP::BLUE);
                ThisThread::sleep_for(20);
            }
        }

        // Accend one strip at a time
        for(int w=0;w<2;w++){
            for(int q=0;q<6;q++){
                latchedLEDs.write_strip(0,LEDGROUP::RED);
                latchedLEDs.write_strip(0,LEDGROUP::GREEN);
                latchedLEDs.write_strip(0,LEDGROUP::BLUE);
                for(int i=0;i<8;i++){
                    LEDGROUP group;
                    if(q==0 || q==5){
                        group = LEDGROUP::RED;
                    }
                    else if(q==1 || q==4){
                        group = LEDGROUP::GREEN;
                    }
                    else if(q==2 || q==3){
                        group = LEDGROUP::BLUE;
                    }
                    if(q<3){
                        latchedLEDs.write_strip(1<<i,group);
                    }
                    else{
                        latchedLEDs.write_strip(128>>i,group);
                    }
                    ThisThread::sleep_for(20ms);
                }
            }
        }
    }
}

// Function: maxtrix_scan (Thread)
// Displays a number of test patterns on the matrix display
void matrix_scan(void)
{
    /********** LED matrix Sensor *********/

    while(1){
        // The first pattern use the matrix.dot(row,col) function
        // This places a dot at the coordinates provided
        for(int i=0;i<8;i++){
            for(int x=0;x<16;x++){
                matrix.dot(i,x);
                ThisThread::sleep_for(10ms);
            }
        }

        for(int i=7;i>=0;i--){
            for(int x=15;x>=0;x--){
                matrix.dot(i,x);
                ThisThread::sleep_for(10ms);
            }
        }

        // The second pattern use the matrix.line(row) function
        // This places a horizontal line on the row provided
        for(int q=0;q<2;q++){
            for(int i=0;i<8;i++){
                matrix.line(i);
                ThisThread::sleep_for(50ms);
            }
        
            for(int i=6;i>=0;i--){
                matrix.line(i);
                ThisThread::sleep_for(50ms);
            }
        }
    }
}

// Function: sevenseg_count_thread (Thread)
// Increments from 0-99 on the seven segment display
void sevenseg_count_thread(){

    /********** Seven segment display (Latched LEDs) *********/

    latchedLEDs.sevenSegclear();
    unsigned char counter=0;
    while(true){
        latchedLEDs.enable(true);
        latchedLEDs.write_seven_seg(counter);
        counter++;
        if (counter>99){counter=0;}
        thread_sleep_for(250);
    }
}

// Function: environment_data (Thread)
// Periodically samples temperature, humidity and pressure
// data and displays it on the terminal 
void environment_data(void){ 

    /********** Environmental Sensor *********/

    float temperature,pressure, humidity;
    ENV_SENSOR_TYPE sensor_type = env.getSensorType();
    
    while(true)
    {
        if(sensor_type==NONE){
            break;
        }
        temperature = env.getTemperature();
        pressure=env.getPressure();
        humidity = env.getHumidity();

        printf("Temperature = %4.1f ",temperature);
        printf("Humidity = %4.1f ",humidity);
        printf("Pressure = %4.1f\n",pressure);
        printf("DIP Switches = %d%d%d%d\n",dipSwitches[0],dipSwitches[1],dipSwitches[2],dipSwitches[3]);
        thread_sleep_for(ENV_SAMPLE_DELAY);
    }
}

// Function: dac_thread (Thread)
// Outputs sawtooth waveforms at different amplitudes on the 2 DACs
void dac_thread(void){

    /********** Digital-to-Analog Converters *********/

    float ValDAC;

    while(true){
        ValDAC=0.0f;
        for (int i=0; i<=33; i++){
            dac_out(0,ValDAC);
            dac_out(1,ValDAC/2.0f);
            ValDAC+=0.03030303f;
            ThisThread::sleep_for(20ms);        
        }
    }
}
// Function: dac_out
// Helper function for selecting which DAC to update
void dac_out(int dac_num, float DACVal){

    switch (dac_num){
       case 0: dac_out_1.write(DACVal);break;
       case 1: dac_out_2.write(DACVal);break;
        default: break;
    }
}