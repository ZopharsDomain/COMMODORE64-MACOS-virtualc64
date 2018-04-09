/*!
 * @header      CIA.h
 * @author      Dirk W. Hoffmann, www.dirkwhoffmann.de
 * @copyright   2006 - 2018 Dirk W. Hoffmann
 */
/*              This program is free software; you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation; either version 2 of the License, or
 *              (at your option) any later version.
 *
 *              This program is distributed in the hope that it will be useful,
 *              but WITHOUT ANY WARRANTY; without even the implied warranty of
 *              MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *              GNU General Public License for more details.
 *
 *              You should have received a copy of the GNU General Public License
 *              along with this program; if not, write to the Free Software
 *              Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef _CIA_INC
#define _CIA_INC

#include "TOD.h"

// Forward declarations
class VIC;
class IEC;
class Keyboard;
class Joystick;

// Adapted from PC64WIN
#define CountA0     0x00000001
#define CountA1     0x00000002
#define CountA2     0x00000004
#define CountA3     0x00000008
#define CountB0     0x00000010
#define CountB1     0x00000020
#define CountB2     0x00000040
#define CountB3     0x00000080
#define LoadA0      0x00000100
#define LoadA1      0x00000200
#define LoadA2      0x00000400
#define LoadB0      0x00000800
#define LoadB1      0x00001000
#define LoadB2      0x00002000
#define PB6Low0     0x00004000
#define PB6Low1     0x00008000
#define PB7Low0     0x00010000
#define PB7Low1     0x00020000
#define Interrupt0  0x00040000
#define Interrupt1  0x00080000
#define OneShotA0   0x00100000
#define OneShotB0   0x00200000
#define ReadIcr0    0x00400000
#define ReadIcr1    0x00800000
#define ClearIcr0   0x01000000
#define ClearIcr1   0x02000000
#define ClearIcr2   0x04000000
#define SetIcr0     0x08000000
#define SetIcr1     0x10000000
#define TODInt0     0x20000000

#define Cnt0        0x0000100000000000
#define Cnt1        0x0000200000000000
#define Cnt2        0x0000400000000000
#define SerInt0     0x0000800000000000
#define SerInt1     0x0001000000000000
#define SerInt2     0x0002000000000000
#define SerLoad0    0x0004000000000000
#define SerLoad1    0x0008000000000000
#define SerClk0     0x0010000000000000
#define SerClk1     0x0020000000000000
#define SerClk2     0x0040000000000000
#define SerClk3     0x0080000000000000

// Hoxs
#define SetCntFlip0   0x0000000400000000ULL
#define SetCntFlip1   0x0000000800000000ULL
#define SetCntFlip2   0x0000001000000000ULL
#define SetCntFlip3   0x0000002000000000ULL
#define SetCnt0       0x0000004000000000ULL
#define SetCnt1       0x0000008000000000ULL
#define SetCnt2       0x0000010000000000ULL
#define SetCnt3       0x0000020000000000ULL



#define DelayMask ~(CountA0 | CountB0 | LoadA0 | LoadB0 | PB6Low0 | PB7Low0 | Interrupt0 | OneShotA0 | OneShotB0 | ReadIcr0 | ClearIcr0 | SetIcr0 | TODInt0 | Cnt0 | SerInt0 | SerLoad0 | SerClk0 | /* Hoxs */ SetCntFlip0 | SetCnt0)


/*! @brief    Virtual complex interface adapter (CIA)
 *  @details  The original C64 consists of two CIA chips (CIA 1 and CIA 2). Each CIA chip features two programmable
 *            hardware timers and a real-time clock. Furthermore, the CIA chips manage the communication with connected
 *            peripheral devices such as joysticks, printer or the keyboard. The CIA class implements the common
 *            functionality of both CIA chips.
 */
class CIA : public VirtualComponent {


    uint8_t SDR;
    bool serClk;
    uint8_t serCounter; 

    // From Hoxs
    
    // Replace by serClock
    uint8_t serial_int_count;
    
    // Replace by SerInt0 SerInt1 SerInt2
    // uint32_t serial_interrupt_delay;
    
    // Replace by Cnt0, Cnt1???
    bool f_cnt_out;
    
    // ---------------------------------------------------------------------------------------
    //                                          Properties
    // ---------------------------------------------------------------------------------------

public:
    
	//! @brief    Start address of the CIA I/O space (CIA 1 and CIA 2)
	static const uint16_t CIA_START_ADDR = 0xDC00;

    //! @brief    End address of the CIA I/O space (CIA 1 and CIA 2)
	static const uint16_t CIA_END_ADDR = 0xDDFF;
    
    bool todAlarm;
    
	//! @brief    Timer A counter
	uint16_t counterA;
	
	//! @brief    Timer A latch
	uint16_t latchA;

	//! @brief    Timer B counter
	uint16_t counterB;
	
	//! @brief    Timer B latch
	uint16_t latchB;
	
	//! @brief    Time of day clock
	TOD tod;
	
public:	
	
	// 
	// Adapted from PC64Win by Wolfgang Lorenz
	//
		
    //
	// Control
    //
    
    //! @brief    Performs delay by shifting left at each clock
	uint64_t delay;
    
    //! @brief    New bits to feed into dwDelay
	uint64_t feed;
    
    //! @brief    Control register A
	uint8_t CRA;

    //! @brief    Control register B
    uint8_t CRB;
    
    //! @brief    Interrupt control register
	uint8_t ICR;
    
    //! @brief    Interrupt mask register
	uint8_t IMR;

    //! @brief    Bit mask for PB outputs: 0 = port register, 1 = timer
    uint8_t PB67TimerMode;
    
    //! @brief    PB outputs bits 6 and 7 in timer mode
	uint8_t PB67TimerOut;
    
    //! @brief    PB outputs bits 6 and 7 in toggle mode
	uint8_t PB67Toggle;
		
	//
    // Ports
    //
    
    //! @brief    Bbuffered output value of port A
    uint8_t PALatch;

    //! @brief    Bbuffered output value of port B
    uint8_t PBLatch;
    
    //! @brief    Data directon register for port A (0 = input, 1 = output)
	uint8_t DDRA;

    //! @brief    Data directon register for port B (0 = input, 1 = output)
    uint8_t DDRB;
	
    //
	// Interfaces
    //
    
	uint8_t PA;
	uint8_t PB;
    
    //! @brief    Serial clock or input timer clock or timer gate
	bool CNT;
	bool INT;

	/*! @brief    Requests the CPU to interrupt
	 *  @details  This function is abstract and implemented differently by CIA1 and CIA2.
     *            CIA 1 activates the IRQ line and CIA 2 the NMI line.
     */
	virtual void pullDownInterruptLine() = 0;

	/*! @brief    Removes the interrupt requests
	 *  @details  This function is abstract and implemented differently by CIA1 and CIA2.
     *            CIA 1 clears the IRQ line and CIA 2 the NMI line.
     */
	virtual void releaseInterruptLine() = 0;
    
    
    // ------------------------------------------------------------------------------------------
    //                                             Methods
    // ------------------------------------------------------------------------------------------

public:	
	
	//! @brief    Returns true if addr is located in the I/O range of one of the two CIA chips
	static bool isCiaAddr(uint16_t addr) { return (CIA_START_ADDR <= addr && addr <= CIA_END_ADDR); }
	
	//! @brief    Constructor
	CIA();
	
	//! @brief    Destructor
	~CIA();
	
	//! @brief    Bring the CIA back to its initial state
	void reset();
    	
	//! @brief    Dump internal state
	void dumpState();	

	//! @brief    Dump trace line
	void dumpTrace();	

    
    //
    //! @functiongroup Accessing device properties
    //
    
	//! @brief    Returns the value of data port A
    uint8_t getDataPortA() { return peek(0x00); }

	//! @brief    Sets the current value of data port A
    void setDataPortA(uint8_t value) { poke(0x00, value); }

	//! @brief    Returns the value of the data port A direction register
    uint8_t getDataPortDirectionA() { return DDRA; }
	
	//! @brief    Sets the current value of the data port A direction register
    void setDataPortDirectionA(uint8_t value) { DDRA = value; }
	
	//! @brief    Returns the value of data port B
    uint8_t getDataPortB() { return PB; }
	
	//! @brief    Sets the current value of data port B
    void setDataPortB(uint8_t value) { poke(0x01, value); }
	
	//! @brief    Returns the value of the data port B direction register
    uint8_t getDataPortDirectionB() { return DDRB; }
	
	//! @brief    Sets the current value of the data port B direction register
    void setDataPortDirectionB(uint8_t value) { DDRB = value; }

    //! @brief    Simulates a rising edge on the flag pin
    void triggerRisingEdgeOnFlagPin();

    //! @brief    Simulates a falling edge on the flag pin
    void triggerFallingEdgeOnFlagPin();
    
    //
	// Interrupt control
	//
    
	//! @brief    Returns true, if timer can trigger interrupts
    bool isInterruptEnabledA() { return IMR & 0x01; }

	//! @brief    Sets or deletes interrupt enable flag
    void setInterruptEnabledA(bool b) { if (b) IMR |= 0x01; else IMR &= (0xff-0x01); }

	//! @brief    Toggles interrupt enable flag of timer A
    void toggleInterruptEnableFlagA() { setInterruptEnabledA(!isInterruptEnabledA()); }

	//! @brief    Returns true, if timer A has reached zero
    bool isSignalPendingA() { return ICR & 0x01; }

	//! @brief    Sets or delete signal pending flag
    void setSignalPendingA(bool b) { if (b) ICR |= 0x01; else ICR &= (0xff-0x01); }

	//! @brief    Toggles signal pending flag of timer A
    void togglePendingSignalFlagA() { setSignalPendingA(!isSignalPendingA()); }
		
	//! @brief    Returns true, if timer B can trigger interrupts
    bool isInterruptEnabledB() { return IMR & 0x02; }

	//! @brief    Sets or deletes interrupt enable flag
    void setInterruptEnabledB(bool b) { if (b) IMR |= 0x02; else IMR &= (0xff-0x02); }

	//! @brief    Toggles interrupt enable flag of timer B
    void toggleInterruptEnableFlagB() { setInterruptEnabledB(!isInterruptEnabledB()); }

	//! @brief    Returns true, if timer B has reached zero
    bool isSignalPendingB() { return ICR & 0x02; }

	//! @brief    Sets or delete signal pending flag
    void setSignalPendingB(bool b) { if (b) ICR |= 0x02; else ICR &= (0xff-0x02); }
	
	//! @brief    Toggles signal pending flag of timer B
    void togglePendingSignalFlagB() { setSignalPendingB(!isSignalPendingB()); }

	//! @brief    Returns true, if the "time of day" interrupt alarm is enabled
    bool isInterruptEnabledTOD() { return ICR & 0x04; }

	//! @brief    Enables or disable "time of day" interrupts
    void setInterruptEnabledTOD(bool b) { if (b) ICR |= 0x04; else ICR &= (0xff-0x04); }

    //! @brief    Returns true, if a negative edge on the FLAG pin triggers an interrupt
    bool isInterruptEnabledFlg() { return ICR & 0x10; }
    
    //! @brief    Enables or disable interrupts on negative edges of the FLAG pin
    void setInterruptEnabledFlg(bool b) { if (b) ICR |= 0x10; else ICR &= (0xff-0x10); }
    
	//
	// Timer A
	// 
	
	//! @brief    Returns latch value.
    uint16_t getLatchA() { return latchA; }
	
	//! @brief    Sets latch value.
    void setLatchA(uint16_t value) { latchA = value; }
	
	//! @brief    Returns low byte of latch.
    uint8_t getLatchALo() { return (uint8_t)(latchA & 0xFF); }
	
	//! @brief    Sets low byte of latch.
    void setLatchALo(uint8_t value) { latchA = (latchA & 0xFF00) | value; }
	
	//! @brief    Returns high byte of latch.
    uint8_t getLatchAHi() { return (uint8_t)(latchA >> 8); }
	
	//! @brief    Sets high byte of latch.
    void setLatchAHi(uint8_t value) { latchA = (value << 8) | (latchA & 0xFF); }
	
	//! @brief    Returns current timer value.
    uint16_t getCounterA() { return counterA; }
	
	//! @brief    Sets current timer value.
    void setCounterA(uint16_t value) { counterA = value; }
	
	//! @brief    Returns low byte of current timer value.
    uint8_t getCounterALo() { return (uint8_t)(counterA & 0xFF); }
	
	//! @brief    Sets low byte of current timer value.
    void setCounterALo(uint8_t value) { counterA = (counterA & 0xFF00) | value; }
	
	//! @brief    Returns high byte of current timer value.
    uint8_t getCounterAHi() { return (uint8_t)(counterA >> 8); }
	
	//! @brief    Sets high byte of current timer value.
    void setCounterAHi(uint8_t value) { counterA = (value << 8) | (counterA & 0xFF); }
	
	/*! @brief    Load latched value into timer.
	 *  @details  As a side effect, CountA2 is cleared. This causes the timer to wait
     *            for one cycle before it continues to count.
     */
    void reloadTimerA() { counterA = latchA; delay &= ~CountA2; }
	
	//! @brief    Returns true, if timer is running, 0 if stopped.
    bool isStartedA() { return CRA & 0x01; }
	
	//! @brief    Starts or stops timer.
    void setStartedA(bool b) { if (b) CRA |= 0x01; else CRA &= 0xFE; }
	
	//! @brief    Toggles start flag.
    void toggleStartFlagA() { setStartedA(!isStartedA()); }
	
	//! @brief    Returns true, if the force load strobe is 1.
    bool forceLoadStrobeA() { return CRA & 0x10; }
	
	//! @brief    Returns true, if an underflow will be indicated in bit #6 in Port B register.
    bool willIndicateUnderflowA() { return CRA & 0x02; }
	
	//! @brief    Returns true, if an underflow will be indicated as a single pulse.
    bool willIndicateUnderflowAsPulseA() { return !(CRA & 0x04); }
	
	//! @brief    Enables or disables underflow indication.
    void setIndicateUnderflowA(bool b) { if (b) CRA |= 0x02; else CRA &= (0xFF-0x02); }
	
	//! @brief    Toggles underflow indication flag.
    void toggleUnderflowFlagA() { setIndicateUnderflowA(!willIndicateUnderflowA()); }
	
	//! @brief    Returns true, if timer is in one shot mode.
    bool isOneShotA() { return CRA & 0x08; }
	
	//! @brief    Enables or disables one-shot-mode.
    void setOneShotA(bool b) { if (b) CRA |= 0x08; else CRA &= (0xff-0x08); }
	
	//! @brief    Toggle one shot flag.
    void toggleOneShotFlagA() { setOneShotA(!isOneShotA()); }
	
	//! @brief    Returns true, if timer counts clock ticks.
    bool isCountingClockTicksA() { return (CRA & 0x20) == 0x00; }
	
	//! @brief    Returns value of timer control register.
    bool getControlRegA() { return CRA; }

	//! @brief    Sets value of timer control register.
    void setControlRegA(uint8_t value) { CRA = value; }
	
	//
	// Timer B
	// 
	
	//! @brief    Returns latch value.
    uint16_t getLatchB() { return latchB; }
	
	//! @brief    Sets latch value.
    void setLatchB(uint16_t value) { latchB = value; }
	
	//! @brief    Returns low byte of latch.
    uint8_t getLatchBLo() { return (uint8_t)(latchB & 0xFF); }
	
	//! @brief    Set low byte of latch.
    void setLatchBLo(uint8_t value) { latchB = (latchB & 0xFF00) | value; }
	
	//! @brief    Returns high byte of latch.
    uint8_t getLatchBHi() { return (uint8_t)(latchB >> 8); }
	
	//! @brief    Set high byte of latch.
    void setLatchBHi(uint8_t value) { latchB = (value << 8) | (latchB & 0xFF); }
	
	//! @brief    Returns current timer value.
    uint16_t getCounterB() { return counterB; }
	
	//! @brief    Set current timer value.
    void setCounterB(uint16_t value) { counterB = value; }
	
	//! @brief    Returns low byte of current timer value.
    uint8_t getCounterBLo() { return (uint8_t)(counterB & 0xFF); }
	
	//! @brief    Set low byte of current timer value.
    void setCounterBLo(uint8_t value) { counterB = (counterB & 0xFF00) | value; }
	
	//! @brief    Returns high byte of current timer value.
    uint8_t getCounterBHi() { return (uint8_t)(counterB >> 8); }
	
	//! @brief    Set high byte of current timer value.
    void setCounterBHi(uint8_t value) { counterB = (value << 8) | (counterB & 0xFF); }
	
	/*! @brief    Loads latched value into timer.
	 *  @details  As a side effect, CountB2 is cleared. This causes the timer to wait for
     *            one cycle before it continues to count.
     */
    void reloadTimerB() { counterB = latchB; delay &= ~CountB2; }
	
	//! @brief    Returns true, if timer is running, 0 if stopped.
    bool isStartedB() { return CRB & 0x01; }
	
	//! @brief    Starts or stop timer.
    void setStartedB(bool b) { if (b) CRB |= 0x01; else CRB &= 0xFE; }
	
	//! @brief    Toggles start flag.
    void toggleStartFlagB() { setStartedB(!isStartedB()); }
	
	//! @brief    Returns true, if the force load strobe is 1.
    bool forceLoadStrobeB() { return CRB & 0x10; }
	
	//! @brief    Returns true, if an underflow will be indicated in bit #7 in Port B register.
    bool willIndicateUnderflowB() { return CRB & 0x02; }
	
	//! @brief    Returns true, if an underflow will be indicated as a single pulse.
    bool willIndicateUnderflowAsPulseB() { return !(CRB & 0x04); }
	
	//! @brief    Enables or disables underflow indication.
    void setIndicateUnderflowB(bool b) { if (b) CRB |= 0x02; else CRB &= (0xFF-0x02); }
	
	//! @brief    Toggles underflow indication flag.
    void toggleUnderflowFlagB() { setIndicateUnderflowB(!willIndicateUnderflowB()); }
	
	//! @brief    Returns true, if timer is in one shot mode.
    bool isOneShotB() { return CRB & 0x08; }
	
	//! @brief    Enables or disable one-shot-mode.
    void setOneShotB(bool b) { if (b) CRB |= 0x08; else CRB &= (0xff-0x08); }
	
	//! @brief    Toggles one shot flag.
    void toggleOneShotFlagB() { setOneShotB(!isOneShotB()); }
	
	//! @brief    Returns true, if timer counts clock ticks.
    bool isCountingClockTicksB() { return (CRB & 0x20) == 0x00; }
	
	//! @brief    Returns value of timer control register.
    bool getControlRegB() { return CRB; }
	
	//! @brief    Sets value of timer control register.
    void setControlRegB(uint8_t value) { CRB = value; }
	
	
    //
    //! @functiongroup Communicating via the I/O address space
    //
    
    /*! @brief    Special peek function for the I/O memory range
     *  @details  The peek function only handles those registers that are treated similarily by the CIA 1 and CIA 2 chip
     */
    virtual uint8_t peek(uint16_t addr);
    
    /*! @brief    Special poke function for the I/O memory range
     *  @details  The poke function only handles those registers that are treated similarily by the CIA 1 and CIA 2 chip
     */
    virtual void poke(uint16_t addr, uint8_t value);
    
    
    //
    //! @functiongroup Running the device
    //
    
	//! @brief    Executes the CIA for one cycle
	void executeOneCycle();

	//! @brief    Increments the TOD clock by one tenth of a second
	void incrementTOD();

    //! Triggers a TOD interrupt if current time matches alarm time
    void checkForTODInterrupt();
};


/*! @class    The first virtual complex interface adapter (CIA 1)
 *  @details  The CIA 1 chips differs from the CIA 2 chip in several smaller aspects. For example, 
 *            the CIA 1 interrupts the CPU via the IRQ line (maskable interrupts). Furthermore, 
 *            the keyboard is connected to the the C64 via the CIA 1 chip.
 */
class CIA1 : public CIA {
	
public:
	
	//! @brief    Start address of the CIA 1 I/O space
	static const uint16_t CIA1_START_ADDR = 0xDC00;
    
	//! @brief    End address of the CIA 1 I/O space
	static const uint16_t CIA1_END_ADDR = 0xDCFF;

	//! @brief    Joystick bits
	uint8_t joystick[2];
	
private:

    //! @brief    Polls current state of a single joystick
    void pollJoystick(Joystick *joy, int joyDevNo);

    //! @brief    Requests an IRQ
    void pullDownInterruptLine();
    
    //! @brief    Releases the IRQ request
    void releaseInterruptLine();

public:

	//! @brief    Constructor
	CIA1();

	//! @brief    Destructor
	~CIA1();
	
	//! @brief    Restores the initial state
	void reset();
		
	//! @brief    Returns true if addr is located in the I/O range of the CIA 1 chip
	static bool isCia1Addr(uint16_t addr)
		{ return (CIA1_START_ADDR <= addr && addr <= CIA1_END_ADDR); }
	
    //! @brief    Custom implementation of peek
	uint8_t peek(uint16_t addr);

    //! @brief    Custom implementation of poke
    void poke(uint16_t addr, uint8_t value);
	
	/*! @brief    Simulates a joystick movement
	 *  @param    nr    joystick number (1 or 2)
     *  @param    value bit pattern of joystick movement
     */
	void setJoystickBits(int nr, uint8_t mask);

    //! @brief    Clears all joystick bits
    void clearJoystickBits(int nr, uint8_t mask);

    //! @brief    Prints debug information
	void dumpState();
};
	
/*! @brief    The second virtual complex interface adapter (CIA 2)
 *  @details  The CIA 2 chips differs from the CIA 1 chip in several smaller aspects. For example,
 *            the CIA 2 interrupts the CPU via the NMI line (non maskable interrupts). Furthermore, 
 *            the CIA 2 controlls the memory bank seen by the video controller. Therefore, CIA 2 
 *            needs to know about the VIC chip, in contrast to CIA 1.
 */
class CIA2 : public CIA {

public:
	
	//! @brief    Start address of the CIA 2 I/O space
	static const uint16_t CIA2_START_ADDR = 0xDD00;
    
	//! @brief    End address of the CIA 1 2/O space
	static const uint16_t CIA2_END_ADDR = 0xDDFF;

private:

    //! @brief    Requests an NMI
    void pullDownInterruptLine();
    
    //! @brief    Releases the NMI request
    void releaseInterruptLine();
    
public:

	//! Constructor
	CIA2();
	
	//! Destructor
	~CIA2();
	
	//! Bring the CIA back to its initial state
	void reset();
	
	//! Returns true if the \a addr is located in the I/O range of the CIA 2 chip
	static bool isCia2Addr(uint16_t addr) 
		{ return (CIA2_START_ADDR <= addr && addr <= CIA2_END_ADDR); }

	uint8_t peek(uint16_t addr);
	void poke(uint16_t addr, uint8_t value);
	
	void dumpState();
};

#endif
