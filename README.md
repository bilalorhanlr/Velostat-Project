# Velostat-Project

## Introduction

Velostat is a polymeric material that is commonly used in the production of pressure-sensitive resistor (PSR) sensors. It is produced by adding carbon black powders into polyvinylidene fluoride (PVF) films, and this material creates changes in its electrical resistance when pressure is applied. Velostat provides electrical insulation but is flexible and lightweight, making it frequently used in electronic applications. It is especially commonly used in the production of pressure-sensitive sensors, particularly in applications such as robotics, medical devices, and wearable technology. 
In this project, we addressed posture problems. We created a project that evaluates the data based on the pressure on the back and hips and provides information on whether the user is sitting correctly or not.

<p align="center">
  <img src="https://github.com/bilalorhanlr/Velostat-Project/blob/main/Pictures/photo5.jpeg" width="300" title="hover text">
</p>

## List of Materials

- 2x Velostat sheet
- 2x ESP32
- 8x Analog Multiplexer
- 120x 1k resistor
- 8x 2k resistor
- 40 meters of 5mm copper tape
- 30-pinned ribbon cable
- 2x Perfboard

## Source Code

The program works in two stages. The first stage is the data collection stage, which we run in mod 0. We obtain data by sitting or leaning on the sheet.
```c
 int mod = 0; //-> veri toplama
 //int mod = 1; //-> veri işleme
```

we saving this data in the setup2 function
```c
void setup2()
{
    string_update("tamamen duzgun", 0);
    string_update("yarim dogru", 1);
    all_update(0, "130 101 194 164 225 229 124 44 153 191 ...");
    all_update(1, "250 320 492 541 601 632 684 618 604 555 ... ");
}
```

we run mod 1 for the second stage, which involves defining the collected data.
```c
 //int mod = 0; //-> veri toplama
 int mod = 1; //-> veri işleme
```
## Electronic

We stick a horizontal copper tape on one perfboard and a vertical copper tape on another perfboard. We place the velostat between these two perfboards and connect each copper trace to a multiplexer with a wire.
<p align="center">
  <img src="https://github.com/bilalorhanlr/Velostat-Project/blob/main/Pictures/Layers.jpeg" width="300" title="hover text">
  <img src="https://github.com/bilalorhanlr/Velostat-Project/blob/main/Schematic/schematic2.png" width="681" title="hover text">
</p>



###### NOTE

If you are going to do this project using STM, you can get support from these resources.
- [Mux Usage](https://www.stm32duino.com/viewtopic.php?t=1073)
(You may have completed this while you are reading this. Please don't hesitate to contact me if you have any questions or concerns.)


###### RESOURCES

- [cd74hc4067 Multiplexer](https://www.ti.com/lit/ds/symlink/cd74hc4067.pdf)
