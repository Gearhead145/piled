*Note - For best appearance, view with a monospaced font with word-wrap disabled* 



                 ___      _______  ______     _______  ___   ___      _______ 
                |   |    |       ||      |   |       ||   | |   |    |       |
                |   |    |    ___||  _    |  |_     _||   | |   |    |    ___|
                |   |    |   |___ | | |   |    |   |  |   | |   |    |   |___ 
                |   |___ |    ___|| |_|   |    |   |  |   | |   |___ |    ___|
                |       ||   |___ |       |    |   |  |   | |       ||   |___ 
                |_______||_______||______|     |___|  |___| |_______||_______|

                              More than just another LED desk lamp.




                          - An Open-Source Hardware/Software project -



The LED TILE is a MSP430G2553 based development board with integrated high current constant
voltage LED drivers, input sensors, and fully configurable I/O. Female headers allow easy addition
of sensors and other devices, while shorting blocks can be used to enable or disable any of the on-board 
devices (such as the LED Drivers or Microphone). On board voltage regulators produce 3.3V and 5V, which
are available via a female header for use with added circuits.




Hardware Features [Mainboard] :
	-3 channels (RGB) MOSFET based LED outputs (each 1.5A continuous!)
	-Electret microphone, LM386 based pre-amplifier, hardware envelope and low pass filters
	-Three linear potentiometers
	-Two pushbuttons
	-Female header sockets for EVERY PIN on the microcontroller, allowing easy connection of
		sensors and additional circuits (such as additional buttons, CdS cell, etc)
	-Double Male headers allow you to selectively connect or disconnect ANY pin on the
		microcontroller to the main board circuitry using shorting blocks. This is important
		if you want to use I/O pins from the LED TILE sensors (such as the potentiometers)
		for your own sensors and circuits.
	-Screw Terminals for Input Power (12V), Inter-Tile Serial Communication Lines, LED Outputs
	-On-board voltage regulators produce 3.3V and 5V (and unregulated 12V), which are available 
		via female header for simple interface of custom sensors to the LED TILE
	-Constructed using through-hole components - Even a beginner can solder this board together!
		(also available fully soldered. The processor is surface mount, and is always pre-soldered)



	
MSP430G2553 / Software Features:
	-8 channels of 10bit ADCs (for reading analog values into the microcontroller, such as those
		from potentiometers, light sensors, or audio)
	-24 General Purpose digital I/O pins (for attaching buttons, etc)
	-16Kb of program memory offers space for moderately large programs (for example, the current
		LIGHT TILE demo code, which has several modes, including a smooth RGB fade, an adjustable
		color temperature light source, an RGB mixer, a sound activated DJ style fading light,
		and a variable color/frequency/duty cycle strobe, uses under 3Kb of memory)
	-Internal (enable-able) pull up/down resistors makes attaching a button as simple as connecting
		it from a I/O pin to ground or VCC
	-Integrated hardware UART allows for inter-TILE communication with virtually no cpu overhead. 
		While primarily designed to allow an array of LED TILEs to act as pixels and display 
		patterns, the serial can be used for just about anything.
	-Write new code to make the LED TILE do cool things!
	-We have created helper functions to make tasks (such as reading analog values from sensors) 
		simple and intuitive
	-The processor runs at up to 16MHz (even running at 1MHz, the demo program runs fast enough to
		refresh the LEDs around 4000 times a second)
	-Can be easily programmed via the Spy-Bi-Wire interface. We reccomend using a TI Launchpad as 
		a programmer (these cost only $4.30, including shipping/handling). Simply attach the
		launchpad with four jumpers (VSS,GND,RST,TEST) to the LED TILE, and connect the launchpad
		USB cable to the computer. The free version of Code Composer Studio provides a robust
		IDE and compiler, and can be used to develop code for the LED TILE.




Hardware Features [LEDs and interface board] :
	-One LED TILE complete kit includes 5.5 feet of RGB LED tape. 5.5 feet has 100 LEDs (each of 
		which is an surface mount 5050 RGB LED)
			-RGB LED Specs [each individual LED]:
				-100mW per color (300mW if all three are fully on)
				-Luminous Intensity Red: 800mcd, Green: 1000mcd, Blue: 400mcd
	-LED Tape can be cut every 3 LEDs - use long strips for lighting computers, desks, etc.
	-LED Tape is self adhesive and flexible, allowing creative mounting...
	-The LED TILE LED Interface Board can be used to connect 9 strips (each of 9 leds) to form an
		array of 81 LEDs. This is bright enough to be used as a small desk lamp, and can easily
		provide mood/accent light for a decent sized room. This board has solder pads for each
		strip, which eliminates the need to cut many pieces of wire.


******************************************************************************************************
*           LED TILE Mainboard: Schematic layout of interface components/sensors                     *
******************************************************************************************************
*                                                                                                    *
*                                                                                                    *
*             ┌────Three linear potentiometers [available as analog inputs to the microcontroller]   *
*             │                                                                                      *
*             │                                                                                      *
*         ┌───┼───┐  Two tactile switches          ┌──Female headers for 3.3V,5V,12V,GND             *
*         │   │   │        │                       │                                                 *
*       __│___│___│________│_______________________│_______                                          *
*      | _v  _v  _v   __   │                    ◘  │    [+]<─┐                                       *
*      ||  ||▐▌||  | |()|<─┤                    ◘<─┘    [+]<─┤                                       *
*      ||  ||▐▌||  | |__|  │                    ◘       [+]<─┤                                       *
*      ||  ||  ||  |       │                    ◘       [+]<─┼───Screw terminals for                 *
*      ||▐▌||  ||  |       │       ::::::::::::::<─┐    [+]<─┤   Input Power,Inter-Tile Serial,LEDS  *
*      ||▐▌||  ||  |       │  ┌───>◘◘◘◘◘◘◘◘◘◘◘◘◘◘  │    [+]<─┤                                       *
*      ||  ||  ||  |       │  │    ____            │    [+]<─┤                                       *
*      ||  ||  ||  |       │  │   /    \           │    [+]<─┘                                       *
*      ||  ||  ||  |       │  │   |Mic.|           │       |                                         *
*      ||  ||  ||  |       │  │   \____/           │       |                                         *
*      ||  ||  ||  |       │  │                    │       |                                         *
*      ||  ||  ||▐▌|  __   │  ├───>◘◘◘◘◘◘◘◘◘◘◘◘◘◘  │       |                                         *
*      ||  ||  ||▐▌| |()|<─┘  │    ::::::::::::::<─┤       |                                         *
*      ||__||__||__| |__|     │                    │       |                                         *
*      |______________________│____________________│_______|                                         *
*                             │                    │                                                 *
*                 Single female headers      Double male headers                                     *
*                 Easily attach DIY sensors  [Enable/disable on-board devices]                       *
*                                                                                                    *
******************************************************************************************************





******************************************************************************************************
*                           LED TILE LightBoard: Interface Schematic                                 *
******************************************************************************************************
*   ┌────────────────────────────────────────────────────────────────────────────────────────┬────┐  *
*   │                                                                                        │    │  *
*   v________________________________________________________________________________________v    │  *
*  |○                                                                                        ○|   │  *
*  |○                                                                                        ○|   │  *
*  |○                                                                                        ○|   │  *
*  |○__⌂⌂⌂⌂______⌂⌂⌂⌂______⌂⌂⌂⌂______⌂⌂⌂⌂______⌂⌂⌂⌂______⌂⌂⌂⌂______⌂⌂⌂⌂______⌂⌂⌂⌂______⌂⌂⌂⌂__○|   │  *
*        ^         ^         ^         ^         ^         ^         ^         ^         ^        │  *
*        │         │         │         │         │         │         │         │         │        │  *
*        ├─────────┴─────────┴─────────┴─────────┴─────────┴─────────┴─────────┴─────────┘        │  *
*        │                                                                                        │  *
*        └── Solder pads to connect individual strips of LED Tape                                 │  *
*                                                                                                 │  *
*                                           Drilled solder pad connections for LED TILE Mainboard─┘  *
*                                                                                                    *
******************************************************************************************************





******************************************************************************************************
*                LED TILE LightBoard: Suggested LED Configuration [ 81 RGB LEDs]                     *
******************************************************************************************************
*                                                                                                    *
*     __________________________________________________________________________________________     *
*    |○                                                                                        ○|    *
*    |○                                                                                        ○|    *
*    |○                                                                                        ○|    *
*    |○__⌂⌂⌂⌂______⌂⌂⌂⌂______⌂⌂⌂⌂______⌂⌂⌂⌂______⌂⌂⌂⌂______⌂⌂⌂⌂______⌂⌂⌂⌂______⌂⌂⌂⌂______⌂⌂⌂⌂__○|    *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|       *
*       |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|       *
*       |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|       *
*       |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|       *
*       |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|       *
*       |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|       *
*       |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|       *
*       |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|       *
*       |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|    |┌──┐|       *
*       |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|    |└──┘|       *
*       |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |    |       *
*       |____|    |____|    |____|    |____|    |____|    |____|    |____|    |____|    |____|       *
*                                                                                                    *
*                                                                                                    *
******************************************************************************************************


	