
void lereeprom(){
	EEPROM.readBlock(0, XYOutputEEPROM,2);
	MeiodoX = XYOutputEEPROM[0].value;
	MeiodoY = XYOutputEEPROM[1].value;
	Serial.println("Leuuuuuuuuuuuuuuuuu");
}

void gravareeprom(){
	XYInputEEPROM[0].key = 2;
	XYInputEEPROM[0].value = MeiodoX;
	XYInputEEPROM[1].key = 4;
	XYInputEEPROM[1].value = MeiodoY;
	EEPROM.writeBlock(0, XYInputEEPROM,2);
	Serial.println("Gravou");
}



