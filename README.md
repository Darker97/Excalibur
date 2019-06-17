# Excalibur
Ein Smartes Stirnband


## Verbindungen
### Diagram (Idee)
graph LR
A[User] --> B(Button)
A --> C(APP)
B --> D{Stirnband}
C --> D

### Diagram (Technisch)

sequenceDiagram
User ->> App: wählt Modus
APP ->> BLBoard: Code für den Modus
BLBoard ->> CircuitPlayground: setze Int für Modus
Button ->> CircuitPlayground: setzte Modus +1
Note right of CircuitPlayground: Der Modus kann mit Button oder per App umgestellt werden


### Codes für die Verbindung

|Nummer|FunctionsName  | Beschreibung|
|--|--|--|
| 0 |Standart  | |
| 1| Bremse ||
|2|Heiligenschein||||
|3|Party||
|4|Sound||


## Aufbau der App
