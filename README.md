# Vigilante-Game-Framework
Vigilante Framework (or VFrame for short) is a high level framework built to run with SFML to make 2D games with basic 3D rendering.

## Features
* Sprite Rendering with Frame-by-Frame animation
* Tiled (Texture Repeating) sprites, plus animation!
* Custom Text Rendering
* Full Rectangle and Circle collision with Collision Response
* 3D Graphics (Using Modern OpenGL)
* Game State and Substate Management
* Multipass Post Process Effects

## Structure
The framework itself is found in the **VFrame** directory, the **Example** directory contains an example project which uses multiple substates to demonstrate the various features of the engine.

## Prerequisites
* SFML (Preferably 2.4.0 or Higher) and all the required external libraries needed to build projects with it.
* GLEW (If you want to use the 3D rendering capabilities, otherwise you need to define the VFRAME_NO_3D preprocessor to disable it)
* Chipmunk2D (If you want to use the 2D physics capabilities, otherwise you need to define the VFRAME_NO_PHYSICS preprocessor to disable it)
* (Linux Only) libncurses

![3D Object Rendering](https://i.imgur.com/Q5rFUf9.gif)

![Particle Effects](https://i.imgur.com/SsmpZqM.gif)

![Easing Functions](https://i.imgur.com/jABi5k5.gif)

<img src="https://i.imgur.com/hx5WrbM.gif" width="640">

## Games Featured
* [Gemstone Keeper](http://gemstonekeeper.co.uk/)
* [Dungeon Racer](https://gamepopper.itch.io/dungeon-racer)
* [Berzerkatron](https://gamepopper.itch.io/berzerkatron)

## Feedback
Despite being marked as version 1.0, this framework still has room for improvement. Feel free to get in touch to suggest ways to improve functionality or features to add.

## References
* [HaxeFlixel](https://github.com/HaxeFlixel/flixel)
* [Robert Penner's Easing Functions](http://robertpenner.com/easing/)
* [SFML Game Development (Henrik Vogelius Hansson, Jan Haller)](https://github.com/SFML/SFML-Game-Development-Book)
* [Mastering SFML Game Development (Raimondas Pupius)](https://github.com/PacktPublishing/Mastering-SFML-Game-Development)
