#include <msp430.h>

// Function prototypes
void initGPIO(void);
void initTimer(void);
void delay_ms(unsigned int ms);
void displayShapeInfo(int shapeID);
void flashLED(unsigned int times);
void vibrateMotor(void);

// Shape definitions
#define SHAPE1 1
#define SHAPE2 2
#define SHAPE3 3

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

    // Initialize peripherals
    initGPIO();
    initTimer();

    while (1)
    {
        if ((P1IN & BIT1) == 0) // Check if Button 1 (Shape 1) is pressed
        {
            displayShapeInfo(SHAPE1);
            flashLED(2); // Flash LED twice
            vibrateMotor(); // Vibrate motor
        }
        else if ((P1IN & BIT2) == 0) // Check if Button 2 (Shape 2) is pressed
        {
            displayShapeInfo(SHAPE2);
            flashLED(2); // Flash LED twice
            vibrateMotor(); // Vibrate motor
        }
        else if ((P1IN & BIT3) == 0) // Check if Button 3 (Shape 3) is pressed
        {
            displayShapeInfo(SHAPE3);
            flashLED(2); // Flash LED twice
            vibrateMotor(); // Vibrate motor
        }
    }
}

// Initialize GPIO (e.g., buttons, LED)
void initGPIO(void)
{
    // Configure buttons as inputs with pull-up resistors
    P1DIR &= ~(BIT1 | BIT2 | BIT3); // Set P1.1, P1.2, P1.3 as inputs
    P1REN |= BIT1 | BIT2 | BIT3;    // Enable pull-up/down resistors
    P1OUT |= BIT1 | BIT2 | BIT3;    // Configure as pull-up

    // Configure LED as output
    P1DIR |= BIT0;  // Set P1.0 as output (Green LED)
    P1OUT &= ~BIT0; // Turn off LED initially

    // Configure motor pin as output (adjust based on your hardware setup)
    P2DIR |= BIT1;  // Set P2.1 as output (Motor control)
    P2OUT &= ~BIT1; // Turn off motor initially
}

// Initialize Timer A
void initTimer(void)
{
    TA0CCTL0 = CCIE;          // Enable Timer A interrupt
    TA0CTL = TASSEL_2 + MC_0; // SMCLK, Timer halted
}

// Timer-based delay function
void delay_ms(unsigned int ms)
{
    while (ms > 0)
    {
        TA0CCR0 = 1000 - 1;     // Set Timer A period (1ms, assuming 1MHz SMCLK)
        TA0CTL |= MC_1;         // Start Timer in Up mode
        __bis_SR_register(LPM0_bits + GIE); // Enter Low Power Mode and enable interrupts
        ms--;                   // Decrement ms counter
    }
}

// Timer A0 ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    TA0CTL &= ~MC_1;         // Stop Timer
    TA0CTL |= TACLR;         // Clear Timer
    __bic_SR_register_on_exit(LPM0_bits); // Exit Low Power Mode
}

// Display shape information on LCD (mock function for now)
void displayShapeInfo(int shapeID)
{
    switch (shapeID)
    {
    case SHAPE1:
        // Insert LCD logic for Shape 1
        break;
    case SHAPE2:
        // Insert LCD logic for Shape 2
        break;
    case SHAPE3:
        // Insert LCD logic for Shape 3
        break;
    default:
        // Default message
        break;
    }
}

// Flash LED a specified number of times
void flashLED(unsigned int times)
{
    // Flash, flash, then hold ON logic
    P1OUT ^= BIT0;         // Toggle LED (ON)
    delay_ms(500);         // 500ms delay
    P1OUT ^= BIT0;         // Toggle LED (OFF)
    delay_ms(500);         // 500ms delay
    P1OUT ^= BIT0;         // Toggle LED (ON)
    delay_ms(500);         // 500ms delay
    P1OUT ^= BIT0;         // Toggle LED (OFF)
    delay_ms(500);         // 500ms delay
    P1OUT |= BIT0;         // Set LED to ON (Hold ON state)
}

// Vibrate motor (simple ON/OFF control for now)
void vibrateMotor(void)
{
    P2OUT |= BIT1;  // Turn on motor
    delay_ms(500);  // Vibrate for 500ms
    P2OUT &= ~BIT1; // Turn off motor
}
