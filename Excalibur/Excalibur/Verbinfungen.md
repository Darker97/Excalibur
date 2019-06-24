#  Verbindungen - Zuordnung

switch (Input[1]){
//Rot
case 1:   
FARBE = pixels.Color(255,0,0);
//Grün
case 2:
FARBE = pixels.Color(0,255,0);
//Blau
case 3:
FARBE = pixels.Color(0,0,255);
//Gelb
case 4:
FARBE = pixels.Color(255,128,0);
//Pink
case 5:
FARBE = pixels.Color(220,118,255);
//weiß
default: 
FARBE = pixels.Color(255,255,255);
}
switch (Input[0]){
case 1:   pixels.fill(FARBE, 0, 10);
case 2:   pixels.fill(FARBE, 10, 10);
case 3:   pixels.fill(Farbe, 20, 10);
default:  pixels.clear();
}
