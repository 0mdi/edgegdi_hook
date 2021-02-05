# edgegdi_hook

Let us take a look at BitBlt in gdi32.dll...

***Windows Version 10.0.18363.1350***
![Windows Version 10.0.18363.1350](https://github.com/0mdi/edgegdi_hook/blob/main/images/bitblt_10.0.18363.1350.PNG)  

***Windows Version 10.0.19042.746 (20H2)***  
![Windows Version 10.0.19042.746](https://github.com/0mdi/edgegdi_hook/blob/main/images/bitblt_10.0.19042.746.PNG)  

## Example PoC
We can abuse this to hook various exported gdi32 functions with only one .data patch...
![Example PoC](https://github.com/0mdi/edgegdi_hook/blob/main/images/edgegdi_exe.PNG)
  
 
