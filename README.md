# UnoArduSim_simpleProject
# Interactive IoT Music Application

This project involves developing an interactive application using an Arduino platform to play themed "Happy Birthday" tunes and synchronize hardware actions, such as LED lighting, DC motor vibrations, and displaying messages on an LCD screen. The goal is to demonstrate the capabilities of a simple and accessible IoT system that can be used in creative and educational implementations, while integrating various hardware components for diversified functionalities.

## Features
- Users can select between two "Happy Birthday" tunes - original and jazz - using buttons.
- A buzzer plays the musical tones based on predefined notes, synchronized with visual effects created by sequential lighting of LEDs.
- A potentiometer allows for the adjustment of the song's tempo, affecting the speed of sounds, visual effects, and DC motor vibrations.
- The LCD provides real-time visual feedback, displaying available options and user selections, using SPI communication for message updates.

## Components Used
- **Buzzer (Pin 6)**: Plays the sounds of the selected tunes.
- **DC Motor (Pin 5)**: Activates vibrations during song playback, providing tactile feedback for an enhanced experience.
- **LEDs (Pins 8, 12, 10, 11)**: Four LEDs light up sequentially, creating a visual effect synchronized with the tune.
- **Potentiometer (Slider - Pin A0)**: Allows for adjusting the song's tempo between 1x and 3x, offering flexibility and interactivity to the user.
- **SPI-controlled LCD Screen (Pins 4, 7, 11, 13)**: Displays messages and options during system use, ensuring clear visual communication.
- **Buttons (Pins 2, 9, 1)**:
  - **Button1**: Displays the introductory message and available options.
  - **Button2**: Selects the "Jazz Happy Birthday" tune.
  - **Button3**: Selects the original tune.

## How to Use
1. **Setup**: Connect all components to their respective pins on the Arduino board.
2. **Power On**: Turn on the Arduino system.
3. **Select Tune**: Press the appropriate button to select either the original or jazz "Happy Birthday" tune.
4. **Adjust Tempo**: Use the potentiometer to adjust the tempo of the tune.
5. **Enjoy**: Experience the synchronized audio-visual effects and tactile feedback as the tune plays.


