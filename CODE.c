

int Modus_Auswahl = 0;
int Brems_auswahl = false;

int Sound_Lautstärke = 0;

int SleepModus_Bool = false;

void setup() {
  // put your setup code here, to run once:
    //Bluetooth Verbindung
    attachInterrupt(CircuitPlayground.slideSwitch(), SleepModus, RISING);
    
    attachInterrupt(BlIncoming, Bluetooth, RISING)


    attachInterrupt(dCircuitPlayground.leftButton(), Moduswechsel, RISING);
    //Bei Bremsen auslösen
    attachInterrupt(BREMSE,Brems_Interrupt, RISING);
}

void SleepModus(){
    if (SleepModus_Bool)
        deattachInterrupt(BlIncoming, Bluetooth, RISING)
        deattachInterrupt(dCircuitPlayground.leftButton(), Moduswechsel, RISING);
        deattachInterrupt(BREMSE,Brems_Interrupt, RISING);

        Modus_Auswahl = 0;
        set_sleep_Mode(SLEEP_MODE_PWR_DOWN);
        sleep_Cpu();
    else 
        sleep_disable();
        setup()  
}

void loop() {
  // put your main code here, to run repeatedly:
        switch Modus_Auswahl{
        //Standart
        case 0:

        //Bremse
        case 1:
            LICHTER_AN(Array lichter Seite)
            Ist Brems_Zeit < 5 sec => Brems_auswahl = false

            if Brems_auswahl 
                LICHTER_AN()
            else LICHTER_AUS
        // Heiligenschein
        case 2:
            LICHTER_AN(Array lichter Heiligenschein)
        //Party
        case 3:
            LICHTER_AN()
            LICHTER_AUS()
        //Sound
        case 4:
            clearPixels;
            Sound_Lautstärke = CircuitPlayground.mic.soundPressureLevel(time) / 20;
            LICHTER_AN(Array 1-5)

    }
}


void Moduswechsel(){
    CircuitPlayground.clearPixels();
    Modus_Auswahl++;
    if(Modus_Auswahl == 5){
        Modus_Auswahl = 0;
    }
}

void Brems_Interrupt(){
    //
    Brems_auswahl = true;
    Brems_Zeit = NOW;
    Bluetooth_out("Gebremst");
}

void LICHTER_AN(int [][] LICHTER){
    for (int i = 0; i < LICHTER.length; i++){
        CircuitPlayground.setPixelColor(LICHTER[i][0], LICHTER[i][1], LICHTER[i][2], LICHTER[i][3])
    }
}

void LICHTER_AUS(){
     for (int i = 0; i < LICHTER.length; i++){
        CircuitPlayground.setPixelColor(LICHTER[i][0], 0, 0, 0)
    }
}



//--------------------------------------------------
//Bluetooth Signale

void Bluetooth_Input(int [] Input){
    Modus_Auswahl = 0
    clearPixels
    if Input.count länger als 1 
        LICHTER_AN(Input)
    else 
        if Input = -1
            clearPixels;
        else if Input = -2
            Bluetooth_out(Modus_Auswahl)
        else if Input = -3
            Vibrieren();
        else
            Modus_Auswahl = Input
}

//Sende Daten an Handy
void Bluetooth_out(char [] output){
    SENDE Output;
}


void Vibrieren(){
    VIBRIERE
}