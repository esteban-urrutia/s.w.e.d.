//######################################################################################################################################################################################################## PRE SETUP
//---------------------------------------------------------------------------------------------------- LIBRERIAS
#include <Wire.h>
#include <NewPing.h>
#include <SoftwareSerial.h>
#include <DFPlayerMini_Fast.h>
#include <EEPROM.h>
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- VARIABLES AUXILIARES
#define OFF 0
#define ON 1

#define addr_0_EEPROM 0
#define addr_1_EEPROM 1
#define addr_2_EEPROM 2
#define addr_3_EEPROM 3

unsigned long aux_frecuencia_1 = 0;
unsigned long aux_frecuencia_2 = 0;
unsigned long aux_frecuencia_3 = 0;
unsigned long aux_frecuencia_4 = 0;
unsigned long aux_frecuencia_5 = 0;
unsigned long aux_frecuencia_6 = 0;
unsigned long aux_frecuencia_7 = 0;
unsigned long aux_frecuencia_8 = 0;
unsigned long aux_frecuencia_9 = 0;
unsigned long aux_frecuencia_10 = 0;
unsigned long aux_frecuencia_11 = 0;

float RESISTENCIA_sensor_EC_nivel_2_RECP_EC_menos = 4700;
float RESISTENCIA_sensor_EC_nivel_2_RECP_EC_mas = 4700;

String CALI = "";
unsigned long aux_frecuencia_lectura_EC = 900000;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION DE LOS PERIFERICOS
#define pin_REINICIO_TOTAL_a 12
#define pin_REINICIO_TOTAL_b 13
#define pin_BOMBA_EC_mas 9
#define pin_BOMBA_EC_menos 8

int BOMBA_EC_mas = OFF;
int BOMBA_EC_menos = OFF;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION I2C
int I2C_BUSY = OFF;
String mensaje_por_enviar_al_NodeMcu;
const int largo_mensaje_I2C = 8;
char char_mensaje_recibido_del_NodeMcu[largo_mensaje_I2C + 1];
char char_mensaje_por_enviar_al_NodeMcu[largo_mensaje_I2C + 1];

String reporte = "";
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION SENSOR EC NIVEL 2 RECP EC--
#define pin_5v_sensor_EC_nivel_2_RECP_EC_menos 2
#define pin_sensor_EC_nivel_2_RECP_EC_menos A0
float sensor_PPM_nivel_2_RECP_EC_menos = -1;
float raw_sensor_EC_nivel_2_RECP_EC_menos = 0;
int int_sensor_PPM_nivel_2_RECP_EC_menos = -1;
int buf_menos[10];
int temporal_menos = 0;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION SENSOR NIVEL 1 RECP EC--
#define pin_5v_sensor_nivel_1_RECP_EC_menos 3
#define pin_sensor_nivel_1_RECP_EC_menos A1
int sensor_nivel_1_RECP_EC_menos = -1;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION SENSOR EC NIVEL 2 RECP EC++
#define pin_5v_sensor_EC_nivel_2_RECP_EC_mas 4
#define pin_sensor_EC_nivel_2_RECP_EC_mas A2
float sensor_PPM_nivel_2_RECP_EC_mas = -1;
float raw_sensor_EC_nivel_2_RECP_EC_mas = 0;
int int_sensor_PPM_nivel_2_RECP_EC_mas = -1;
int buf_mas[10];
int temporal_mas = 0;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION SENSOR NIVEL 1 RECP. SOL. NUT.
#define pin_5v_sensor_nivel_1_RECP_SOL_NUT 5
#define pin_sensor_nivel_1_RECP_SOL_NUT A3
int sensor_nivel_1_RECP_SOL_NUT = -1;

int aux_1_MANTENER_NIVEL_ESTABLE_SOL_NUT = OFF;
int aux_2_MANTENER_NIVEL_ESTABLE_SOL_NUT = OFF;
int aux_terminar_MANTENER_NIVEL_ESTABLE_SOL_NUT = OFF;

double millis_V_EC_mas = 0;
double millis_V_EC_menos = 0;
String syEC = "ERR";

double calibracion_EC_mas_Y = 295;
double calibracion_EC_mas_M = 67.7;

double calibracion_EC_menos_Y = 174;
double calibracion_EC_menos_M = 40.8;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION SENSOR PUERTA
#define pin_sensor_ultrasonico_Echo 6
#define pin_sensor_ultrasonico_Trigger 7
#define MAX_DISTANCE 200
int sensor_puerta = -1;

int valor_sensor_ultrasonico_con_puerta_cerrada = 20;

NewPing sensor_ultrasonico(pin_sensor_ultrasonico_Trigger, pin_sensor_ultrasonico_Echo, MAX_DISTANCE);
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION MP3 PLAYER
SoftwareSerial mySerial(10, 11);
DFPlayerMini_Fast myMP3;

int tema_pedido = -1;
bool MP3_BUSY = false;
int puntero_playlist_mp3 = 6;
//____________________________________________________________________________________________________
//########################################################################################################################################################################################################
//######################################################################################################################################################################################################## FUNCIONES
//----------------------------------------------------------------------------------------------------
void I2C_s__RECEIVE(int howMany)A
								{
								 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																			{
																			 char_mensaje_recibido_del_NodeMcu[i] =' ';
																			}
								 int aux = 0;
								 while(Wire.available())
													  {
													   char_mensaje_recibido_del_NodeMcu[aux]= Wire.read();
													   aux = aux + 1;
													  }
								}
//____________________________________________________________________________________________________
//----------------------------------------------------------------------------------------------------
void I2C_s__SEND()
					{
					 Wire.write(mensaje_por_enviar_al_NodeMcu.c_str());
					}
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- I2C PERIFERICOS
void I2C_perifericos(String periferico_abrebiado, String opcion)
						{
						 mensaje_por_enviar_al_NodeMcu = String(String(periferico_abrebiado) + String(",") + String(opcion));
						 mensaje_por_enviar_al_NodeMcu.toCharArray(char_mensaje_por_enviar_al_NodeMcu, largo_mensaje_I2C + 1);
						 I2C_BUSY = ON;

						 aux_frecuencia_11 = millis();
						 while(I2C_BUSY == ON)
											 {
											  Wire.onRequest(I2C_s__SEND);
											  Wire.onReceive(I2C_s__RECEIVE);
											  delay(0);

											  if(char_mensaje_recibido_del_NodeMcu[0] == char_mensaje_por_enviar_al_NodeMcu[0] &&
												 char_mensaje_recibido_del_NodeMcu[1] == char_mensaje_por_enviar_al_NodeMcu[1] &&
												 char_mensaje_recibido_del_NodeMcu[2] == char_mensaje_por_enviar_al_NodeMcu[2] &&
												 char_mensaje_recibido_del_NodeMcu[3] == char_mensaje_por_enviar_al_NodeMcu[3] &&
												 char_mensaje_recibido_del_NodeMcu[4] == char_mensaje_por_enviar_al_NodeMcu[4] &&
												 char_mensaje_recibido_del_NodeMcu[5] == char_mensaje_por_enviar_al_NodeMcu[5] &&
												 char_mensaje_recibido_del_NodeMcu[6] == char_mensaje_por_enviar_al_NodeMcu[6] &&
												 char_mensaje_recibido_del_NodeMcu[7] == char_mensaje_por_enviar_al_NodeMcu[7])
																															 {
																															  I2C_BUSY = OFF;
																															  delay(0);
																															  for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																																										{
																																										 char_mensaje_recibido_del_NodeMcu[i] =' ';
																																										}
																															 }

											  if(millis() - aux_frecuencia_11 > 180000)
																				{
																				 aux_frecuencia_11 = millis();
//																				 INTERRUPTOR("REINICIO_TOTAL", 69);
																				 aux_frecuencia_6 = millis();

																				 I2C_BUSY = OFF;
																				 delay(0);
																				}
											 }
						}
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- I2C LECTURA SENSOR
void I2C_lectura_sensor(String sensor_abrebiado)
				{
				 mensaje_por_enviar_al_NodeMcu = String(String(sensor_abrebiado) + String(",") + String("LEC"));
				 mensaje_por_enviar_al_NodeMcu.toCharArray(char_mensaje_por_enviar_al_NodeMcu, largo_mensaje_I2C + 1);
				 I2C_BUSY = ON;

				 aux_frecuencia_10 = millis();
				 while(I2C_BUSY == ON)
									 {
									  Wire.onRequest(I2C_s__SEND);
									  Wire.onReceive(I2C_s__RECEIVE);
									  delay(0);

									  if(char_mensaje_recibido_del_NodeMcu[0] == char_mensaje_por_enviar_al_NodeMcu[0] &&
										 char_mensaje_recibido_del_NodeMcu[1] == char_mensaje_por_enviar_al_NodeMcu[1] &&
										 char_mensaje_recibido_del_NodeMcu[2] == char_mensaje_por_enviar_al_NodeMcu[2] &&
										 char_mensaje_recibido_del_NodeMcu[3] == char_mensaje_por_enviar_al_NodeMcu[3] &&
										 char_mensaje_recibido_del_NodeMcu[4] == char_mensaje_por_enviar_al_NodeMcu[4])
																													 {
																													  I2C_BUSY = OFF;
																													  delay(0);
																													 }

									  if(millis() - aux_frecuencia_10  > 180000)
																		{
																		 aux_frecuencia_10 = millis();
																		 INTERRUPTOR("REINICIO_TOTAL", 69);
																		 aux_frecuencia_6 = millis();

																		 I2C_BUSY = OFF;
																		 delay(0);
																		}
									 }
				}
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- I2C LECTURA SENSOR ABREBIADO 3 DIGITOS
void I2C_lectura_sensor_abrebiado_3_digitos(String sensor_abrebiado_3_digitos)
				{
				 mensaje_por_enviar_al_NodeMcu = String( String("LECC") + String(",") + String(sensor_abrebiado_3_digitos) );
				 mensaje_por_enviar_al_NodeMcu.toCharArray(char_mensaje_por_enviar_al_NodeMcu, largo_mensaje_I2C + 1);
				 I2C_BUSY = ON;

				 aux_frecuencia_9 = millis();
				 while(I2C_BUSY == ON)
									 {
									  Wire.onRequest(I2C_s__SEND);
									  Wire.onReceive(I2C_s__RECEIVE);
									  delay(0);

									  if(char_mensaje_recibido_del_NodeMcu[4] == char_mensaje_por_enviar_al_NodeMcu[4] &&
										 char_mensaje_recibido_del_NodeMcu[5] == char_mensaje_por_enviar_al_NodeMcu[5] &&
										 char_mensaje_recibido_del_NodeMcu[6] == char_mensaje_por_enviar_al_NodeMcu[6] &&
										 char_mensaje_recibido_del_NodeMcu[7] == char_mensaje_por_enviar_al_NodeMcu[7])
																													 {
																													  I2C_BUSY = OFF;
																													  delay(0);
																													 }

									  if(millis() - aux_frecuencia_9 > 180000)
																		{
																		 aux_frecuencia_9 = millis();
																		 INTERRUPTOR("REINICIO_TOTAL", 69);
																		 aux_frecuencia_6 = millis();

																		 I2C_BUSY = OFF;
																		 delay(0);
																		}
									 }
				}
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INTERRUPTOR
void INTERRUPTOR(String objeto, int estado)
											{
											 if(objeto == "REINICIO_TOTAL" && estado == 69)
																{
																 digitalWrite(pin_REINICIO_TOTAL_a, LOW);
																 digitalWrite(pin_REINICIO_TOTAL_b, LOW);
																 delay(10000);
																 digitalWrite(pin_REINICIO_TOTAL_a, HIGH);
																 digitalWrite(pin_REINICIO_TOTAL_b, HIGH);
																}
											 else if(objeto == "BOMBA_EC_mas" && estado != BOMBA_EC_mas)
																{
																 BOMBA_EC_mas = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_BOMBA_EC_mas, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_BOMBA_EC_mas, HIGH);
																						}
																}
											 else if(objeto == "BOMBA_EC_menos" && estado != BOMBA_EC_menos)
																{
																 BOMBA_EC_menos = estado;
																 if(estado == ON)
																				{
																				 digitalWrite(pin_BOMBA_EC_menos, LOW);
																				}
																 else if(estado == OFF)
																						{
																						 digitalWrite(pin_BOMBA_EC_menos, HIGH);
																						}
																}
											}
//____________________________________________________________________________________________________
//########################################################################################################################################################################################################
//######################################################################################################################################################################################################## SETUP
void setup()
		{
			 Serial.begin(9600);
			 delay(2000);

			 aux_frecuencia_1 = 0;
			 aux_frecuencia_2 = 0;
			 aux_frecuencia_3 = 0;
			 aux_frecuencia_4 = 0;
			 aux_frecuencia_5 = 0;
			 aux_frecuencia_6 = 0;
			 aux_frecuencia_7 = 0;
			 aux_frecuencia_8 = 0;
			 aux_frecuencia_9 = 0;
			 aux_frecuencia_10 = 0;
			 aux_frecuencia_11 = 0;
			 //--------------------------------------------------------------------------------------- PINMODE PARA TODOS LOS PIN'S
			 pinMode(pin_REINICIO_TOTAL_a, OUTPUT);
			 digitalWrite(pin_REINICIO_TOTAL_a, HIGH);
			 
			 pinMode(pin_REINICIO_TOTAL_b, OUTPUT);
			 digitalWrite(pin_REINICIO_TOTAL_b, HIGH);

			 pinMode(pin_BOMBA_EC_mas, OUTPUT);
			 digitalWrite(pin_BOMBA_EC_mas, HIGH);
			 
			 pinMode(pin_BOMBA_EC_menos, OUTPUT);
			 digitalWrite(pin_BOMBA_EC_menos, HIGH);

			 pinMode(pin_sensor_EC_nivel_2_RECP_EC_menos, INPUT);
			 pinMode(pin_sensor_nivel_1_RECP_EC_menos, INPUT);
			 pinMode(pin_sensor_EC_nivel_2_RECP_EC_mas, INPUT);
			 pinMode(pin_sensor_nivel_1_RECP_SOL_NUT, INPUT);

			 pinMode(pin_5v_sensor_EC_nivel_2_RECP_EC_menos, OUTPUT);
			 digitalWrite(pin_5v_sensor_EC_nivel_2_RECP_EC_menos, LOW);

			 pinMode(pin_5v_sensor_nivel_1_RECP_EC_menos, OUTPUT);
			 digitalWrite(pin_5v_sensor_nivel_1_RECP_EC_menos, LOW);
			 
			 pinMode(pin_5v_sensor_EC_nivel_2_RECP_EC_mas, OUTPUT);
			 digitalWrite(pin_5v_sensor_EC_nivel_2_RECP_EC_mas, LOW);
			 
			 pinMode(pin_5v_sensor_nivel_1_RECP_SOL_NUT, OUTPUT);
			 digitalWrite(pin_5v_sensor_nivel_1_RECP_SOL_NUT, LOW);
			 //_______________________________________________________________________________________
			 //--------------------------------------------------------------------------------------- INICIO MP3 PLAYER
			 mySerial.begin(9600);
			 myMP3.begin(mySerial);
			 delay(100);
			 myMP3.play(2);
			 puntero_playlist_mp3 = (int)(EEPROM.read(addr_0_EEPROM)) + (int)(EEPROM.read(addr_1_EEPROM)) + (int)(EEPROM.read(addr_2_EEPROM)) + (int)(EEPROM.read(addr_3_EEPROM));
			 //_______________________________________________________________________________________
			 delay(1000);
			 //--------------------------------------------------------------------------------------- INICIO ENLACE I2C
			 Wire.begin(13);
			 //_______________________________________________________________________________________
			 //--------------------------------------------------------------------------------------- INTERRUPTOR ( todo , off )
			 BOMBA_EC_mas = OFF;
			 BOMBA_EC_menos = OFF;
			 //_______________________________________________________________________________________
			{//--------------------------------------------------------------------------------------- LECTURA SENSOR EC NIVEL 2 RECP EC--
			 digitalWrite(pin_5v_sensor_EC_nivel_2_RECP_EC_menos, HIGH);

			 for(int i=0;i<10;i++) 
								 { 
								 buf_menos[i]=analogRead(pin_sensor_EC_nivel_2_RECP_EC_menos);
								 delay(100);
								 }
			 for( int i = 0 ; i < 9 ; i++ )
										 {
										 for( int j = i + 1 ; j < 10 ; j++ )
																			{
																			 if( buf_menos[i] > buf_menos[j] )
																								 {
																								 temporal_menos=buf_menos[i];
																								 buf_menos[i]=buf_menos[j];
																								 buf_menos[j]=temporal_menos;
																								 }
																			}
										 }
								 
			 raw_sensor_EC_nivel_2_RECP_EC_menos = 0;
			 raw_sensor_EC_nivel_2_RECP_EC_menos = buf_menos[2] + buf_menos[3] + buf_menos[4] + buf_menos[5] + buf_menos[6] + buf_menos[7]; 
			 raw_sensor_EC_nivel_2_RECP_EC_menos = raw_sensor_EC_nivel_2_RECP_EC_menos/6;

			 digitalWrite(pin_5v_sensor_EC_nivel_2_RECP_EC_menos, LOW);	
 
			 sensor_PPM_nivel_2_RECP_EC_menos = ((((1000.0/((((((5.0*raw_sensor_EC_nivel_2_RECP_EC_menos)/1024.0)*(RESISTENCIA_sensor_EC_nivel_2_RECP_EC_menos+25.0))/(5.0-((5.0*raw_sensor_EC_nivel_2_RECP_EC_menos)/1024.0)))-25.0)))))*500.0);

			 int_sensor_PPM_nivel_2_RECP_EC_menos = (int)(sensor_PPM_nivel_2_RECP_EC_menos); // NO compensado por temperatura

			 if(int_sensor_PPM_nivel_2_RECP_EC_menos >= 100 && int_sensor_PPM_nivel_2_RECP_EC_menos < 1000)
								 {
								  I2C_perifericos("2ec-", String(int_sensor_PPM_nivel_2_RECP_EC_menos) );
								 }
			 if(int_sensor_PPM_nivel_2_RECP_EC_menos >= 10 && int_sensor_PPM_nivel_2_RECP_EC_menos < 100)
								 {
								  I2C_perifericos("2ec-", String( String("0") + String(int_sensor_PPM_nivel_2_RECP_EC_menos) ));
								 }
			 if(int_sensor_PPM_nivel_2_RECP_EC_menos < 10)
								 {
								  I2C_perifericos("2ec-", String( String("00") + String(int_sensor_PPM_nivel_2_RECP_EC_menos) ));
								 }
			}
			//________________________________________________________________________________________
			{//--------------------------------------------------------------------------------------- LECTURA SENSOR NIVEL 1 RECP EC--
			 digitalWrite(pin_5v_sensor_nivel_1_RECP_EC_menos, HIGH);

			 if(digitalRead(pin_sensor_nivel_1_RECP_EC_menos) == LOW)
																 {
																  sensor_nivel_1_RECP_EC_menos = ON;
																  digitalWrite(pin_5v_sensor_nivel_1_RECP_EC_menos, LOW);
																  I2C_perifericos("1ec-","001");
																 }

			 else if (digitalRead(pin_sensor_nivel_1_RECP_EC_menos) == HIGH)
																 {
																  sensor_nivel_1_RECP_EC_menos = OFF;
																  digitalWrite(pin_5v_sensor_nivel_1_RECP_EC_menos, LOW);
																  I2C_perifericos("1ec-","000");
																 }
			}
			//________________________________________________________________________________________
			{//--------------------------------------------------------------------------------------- LECTURA SENSOR EC NIVEL 2 RECP EC++
			 digitalWrite(pin_5v_sensor_EC_nivel_2_RECP_EC_mas, HIGH);

			 for(int a=0;a<10;a++) 
								 { 
								 buf_mas[a]=analogRead(pin_sensor_EC_nivel_2_RECP_EC_mas);
								 delay(100);
								 }
			 for( int a = 0 ; a < 9 ; a++ )
										 {
										 for( int b = a + 1 ; b < 10 ; b++ )
																			{
																			 if( buf_mas[a] > buf_mas[b] )
																								 {
																								 temporal_mas=buf_mas[a];
																								 buf_mas[a]=buf_mas[b];
																								 buf_mas[b]=temporal_mas;
																								 }
																			}
										 }
								 
			 raw_sensor_EC_nivel_2_RECP_EC_mas = 0;
			 raw_sensor_EC_nivel_2_RECP_EC_mas = buf_mas[2] + buf_mas[3] + buf_mas[4] + buf_mas[5] + buf_mas[6] + buf_mas[7]; 
			 raw_sensor_EC_nivel_2_RECP_EC_mas = raw_sensor_EC_nivel_2_RECP_EC_mas/6;

			 digitalWrite(pin_5v_sensor_EC_nivel_2_RECP_EC_mas, LOW);
 
			 sensor_PPM_nivel_2_RECP_EC_mas = ((((1000.0/((((((5.0*raw_sensor_EC_nivel_2_RECP_EC_mas)/1024.0)*(RESISTENCIA_sensor_EC_nivel_2_RECP_EC_mas+25.0))/(5.0-((5.0*raw_sensor_EC_nivel_2_RECP_EC_mas)/1024.0)))-25.0)))))*500.0);

			 int_sensor_PPM_nivel_2_RECP_EC_mas = (int)(sensor_PPM_nivel_2_RECP_EC_mas); // x/10 y NO compensado por temperatura

			 if(int_sensor_PPM_nivel_2_RECP_EC_mas >= 100 && int_sensor_PPM_nivel_2_RECP_EC_mas < 1000)
								 {
								  I2C_perifericos("2ec+", String(int_sensor_PPM_nivel_2_RECP_EC_mas) );
								 }
			 if(int_sensor_PPM_nivel_2_RECP_EC_mas >= 10 && int_sensor_PPM_nivel_2_RECP_EC_mas < 100)
								 {
								  I2C_perifericos("2ec+", String( String("0") + String(int_sensor_PPM_nivel_2_RECP_EC_mas) ));
								 }
			 if(int_sensor_PPM_nivel_2_RECP_EC_mas < 10)
								 {
								  I2C_perifericos("2ec+", String( String("00") + String(int_sensor_PPM_nivel_2_RECP_EC_mas) ));
								 }
			}
			//________________________________________________________________________________________
			{//--------------------------------------------------------------------------------------- LECTURA SENSOR NIVEL 1 RECP. SOL. NUT.
			 digitalWrite(pin_5v_sensor_nivel_1_RECP_SOL_NUT, HIGH);

			 if(digitalRead(pin_sensor_nivel_1_RECP_SOL_NUT) == LOW)
																 {
																  sensor_nivel_1_RECP_SOL_NUT = ON;
																  digitalWrite(pin_5v_sensor_nivel_1_RECP_SOL_NUT, LOW);
																  I2C_perifericos("n1sn","001");
																 }

			 else if (digitalRead(pin_sensor_nivel_1_RECP_SOL_NUT) == HIGH)
																 {
																  sensor_nivel_1_RECP_SOL_NUT = OFF;
																  digitalWrite(pin_5v_sensor_nivel_1_RECP_SOL_NUT, LOW);
																  I2C_perifericos("n1sn","000");
																 }
			}
			//________________________________________________________________________________________
			{//--------------------------------------------------------------------------------------- LECTURA SENSOR APERTURA PUERTA
			 sensor_puerta = sensor_ultrasonico.ping_cm();
			 if(sensor_puerta > valor_sensor_ultrasonico_con_puerta_cerrada)
									{
									 I2C_perifericos("door","001");
									}
			 if(sensor_puerta <= valor_sensor_ultrasonico_con_puerta_cerrada)
									{
									 I2C_perifericos("door","000");
									}
			}
			//________________________________________________________________________________________
		}
//########################################################################################################################################################################################################
//######################################################################################################################################################################################################## LOOP
void loop()
		{
			 delay(200);

			{//-------------------------------------------------------------------------------------- LECTURA DE SENSORES

					{//--------------------------------------------------- LECTURA SENSOR EC NIVEL 2 RECP EC--
					 if(millis() - aux_frecuencia_1 > aux_frecuencia_lectura_EC)
														{
														 aux_frecuencia_1 = millis();

														 digitalWrite(pin_5v_sensor_EC_nivel_2_RECP_EC_menos, HIGH);

														 for(int i=0;i<10;i++) 
																			 { 
																			 buf_menos[i]=analogRead(pin_sensor_EC_nivel_2_RECP_EC_menos);
																			 delay(100);
																			 }
														 for( int i = 0 ; i < 9 ; i++ )
																					 {
																					 for( int j = i + 1 ; j < 10 ; j++ )
																														{
																														 if( buf_menos[i] > buf_menos[j] )
																																			 {
																																			 temporal_menos=buf_menos[i];
																																			 buf_menos[i]=buf_menos[j];
																																			 buf_menos[j]=temporal_menos;
																																			 }
																														}
																					 }
																			 
														 raw_sensor_EC_nivel_2_RECP_EC_menos = 0;
														 raw_sensor_EC_nivel_2_RECP_EC_menos = buf_menos[2] + buf_menos[3] + buf_menos[4] + buf_menos[5] + buf_menos[6] + buf_menos[7]; 
														 raw_sensor_EC_nivel_2_RECP_EC_menos = raw_sensor_EC_nivel_2_RECP_EC_menos/6;

                            Serial.println("**********************************************************");
                            Serial.print("raw_sensor_EC_nivel_2_RECP_EC_menos     ");Serial.println(raw_sensor_EC_nivel_2_RECP_EC_menos);
                            Serial.println("");
                            Serial.println("");

														 digitalWrite(pin_5v_sensor_EC_nivel_2_RECP_EC_menos, LOW);
											 
														 sensor_PPM_nivel_2_RECP_EC_menos = ((((1000.0/((((((5.0*raw_sensor_EC_nivel_2_RECP_EC_menos)/1024.0)*(RESISTENCIA_sensor_EC_nivel_2_RECP_EC_menos+25.0))/(5.0-((5.0*raw_sensor_EC_nivel_2_RECP_EC_menos)/1024.0)))-25.0)))))*500.0);

                            Serial.println("");
                            Serial.print("sensor_PPM_nivel_2_RECP_EC_menos     ");Serial.println(sensor_PPM_nivel_2_RECP_EC_menos);
                            Serial.println("");
                            Serial.println("");

														 int_sensor_PPM_nivel_2_RECP_EC_menos = (int)(sensor_PPM_nivel_2_RECP_EC_menos); // x/10 y NO compensado por temperatura

                            Serial.println("");
                            Serial.print("int_sensor_PPM_nivel_2_RECP_EC_menos     ");Serial.println(int_sensor_PPM_nivel_2_RECP_EC_menos);
                            Serial.println("");
                            Serial.println("*****************************************************************");

														 if(int_sensor_PPM_nivel_2_RECP_EC_menos >= 100 && int_sensor_PPM_nivel_2_RECP_EC_menos < 1000)
																			 {
																			  I2C_perifericos("2ec-", String(int_sensor_PPM_nivel_2_RECP_EC_menos) );
																			 }
														 if(int_sensor_PPM_nivel_2_RECP_EC_menos >= 10 && int_sensor_PPM_nivel_2_RECP_EC_menos < 100)
																			 {
																			  I2C_perifericos("2ec-", String( String("0") + String(int_sensor_PPM_nivel_2_RECP_EC_menos) ));
																			 }
														 if(int_sensor_PPM_nivel_2_RECP_EC_menos < 10)
																			 {
																			  I2C_perifericos("2ec-", String( String("00") + String(int_sensor_PPM_nivel_2_RECP_EC_menos) ));
																			 }
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA SENSOR NIVEL 1 RECP EC--
					 if(millis() - aux_frecuencia_2 > aux_frecuencia_lectura_EC)
														{
														 aux_frecuencia_2 = millis();

														 digitalWrite(pin_5v_sensor_nivel_1_RECP_EC_menos, HIGH);

														 if(digitalRead(pin_sensor_nivel_1_RECP_EC_menos) == LOW)
																											 {
																											  sensor_nivel_1_RECP_EC_menos = ON;
																											  digitalWrite(pin_5v_sensor_nivel_1_RECP_EC_menos, LOW);
																											  I2C_perifericos("1ec-","001");
																											 }

														 else if (digitalRead(pin_sensor_nivel_1_RECP_EC_menos) == HIGH)
																											 {
																											  sensor_nivel_1_RECP_EC_menos = OFF;
																											  digitalWrite(pin_5v_sensor_nivel_1_RECP_EC_menos, LOW);
																											  I2C_perifericos("1ec-","000");
																											 }
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA SENSOR EC NIVEL 2 RECP EC++
					 if(millis() - aux_frecuencia_3 > aux_frecuencia_lectura_EC)
														{
														 aux_frecuencia_3 = millis();

														 digitalWrite(pin_5v_sensor_EC_nivel_2_RECP_EC_mas, HIGH);

														 for(int a=0;a<10;a++) 
																			 { 
																			 buf_mas[a]=analogRead(pin_sensor_EC_nivel_2_RECP_EC_mas);
																			 delay(100);
																			 }
														 for( int a = 0 ; a < 9 ; a++ )
																					 {
																					 for( int b = a + 1 ; b < 10 ; b++ )
																														{
																														 if( buf_mas[a] > buf_mas[b] )
																																			 {
																																			 temporal_mas=buf_mas[a];
																																			 buf_mas[a]=buf_mas[b];
																																			 buf_mas[b]=temporal_mas;
																																			 }
																														}
																					 }
																			 
														 raw_sensor_EC_nivel_2_RECP_EC_mas = 0;
														 raw_sensor_EC_nivel_2_RECP_EC_mas = buf_mas[2] + buf_mas[3] + buf_mas[4] + buf_mas[5] + buf_mas[6] + buf_mas[7]; 
														 raw_sensor_EC_nivel_2_RECP_EC_mas = raw_sensor_EC_nivel_2_RECP_EC_mas/6;

                            Serial.println("----------------------------------------------------------------");
                            Serial.print("raw_sensor_EC_nivel_2_RECP_EC_mas     ");Serial.println(raw_sensor_EC_nivel_2_RECP_EC_mas);
                            Serial.println("");
                            Serial.println("");

														 digitalWrite(pin_5v_sensor_EC_nivel_2_RECP_EC_mas, LOW);
											 
														 sensor_PPM_nivel_2_RECP_EC_mas = ((((1000.0/((((((5.0*raw_sensor_EC_nivel_2_RECP_EC_mas)/1024.0)*(RESISTENCIA_sensor_EC_nivel_2_RECP_EC_mas+25.0))/(5.0-((5.0*raw_sensor_EC_nivel_2_RECP_EC_mas)/1024.0)))-25.0)))))*500.0);

                            Serial.println("");
                            Serial.print("sensor_PPM_nivel_2_RECP_EC_mas     ");Serial.println(sensor_PPM_nivel_2_RECP_EC_mas);
                            Serial.println("");
                            Serial.println("");

														 int_sensor_PPM_nivel_2_RECP_EC_mas = (int)(sensor_PPM_nivel_2_RECP_EC_mas); // x/10 y NO compensado por temperatura

                            Serial.println("");
                            Serial.print("int_sensor_PPM_nivel_2_RECP_EC_mas     ");Serial.println(int_sensor_PPM_nivel_2_RECP_EC_mas);
                            Serial.println("");
                            Serial.println("-------------------------------------------------------------");
							Serial.println("");

														 if(int_sensor_PPM_nivel_2_RECP_EC_mas >= 100 && int_sensor_PPM_nivel_2_RECP_EC_mas < 1000)
																			 {
																			  I2C_perifericos("2ec+", String(int_sensor_PPM_nivel_2_RECP_EC_mas) );
																			 }
														 if(int_sensor_PPM_nivel_2_RECP_EC_mas >= 10 && int_sensor_PPM_nivel_2_RECP_EC_mas < 100)
																			 {
																			  I2C_perifericos("2ec+", String( String("0") + String(int_sensor_PPM_nivel_2_RECP_EC_mas) ));
																			 }
														 if(int_sensor_PPM_nivel_2_RECP_EC_mas < 10)
																			 {
																			  I2C_perifericos("2ec+", String( String("00") + String(int_sensor_PPM_nivel_2_RECP_EC_mas) ));
																			 }
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA SENSOR NIVEL 1 RECP. SOL. NUT.
					 if(millis() - aux_frecuencia_4 > aux_frecuencia_lectura_EC)
														{
														 aux_frecuencia_4 = millis();

														 digitalWrite(pin_5v_sensor_nivel_1_RECP_SOL_NUT, HIGH);

														 if(digitalRead(pin_sensor_nivel_1_RECP_SOL_NUT) == LOW)
																											 {
																											  sensor_nivel_1_RECP_SOL_NUT = ON;
																											  digitalWrite(pin_5v_sensor_nivel_1_RECP_SOL_NUT, LOW);
																											  I2C_perifericos("n1sn","001");
																											 }

														 else if (digitalRead(pin_sensor_nivel_1_RECP_SOL_NUT) == HIGH)
																											 {
																											  sensor_nivel_1_RECP_SOL_NUT = OFF;
																											  digitalWrite(pin_5v_sensor_nivel_1_RECP_SOL_NUT, LOW);
																											  I2C_perifericos("n1sn","000");
																											 }
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA SENSOR APERTURA PUERTA
					 if(millis() - aux_frecuencia_5 > 2000)
														{
														 aux_frecuencia_5 = millis();

														 sensor_puerta = sensor_ultrasonico.ping_cm();

														 if(sensor_puerta > valor_sensor_ultrasonico_con_puerta_cerrada)
																				{
																				 I2C_perifericos("door","001");
																				}
														 if(sensor_puerta <= valor_sensor_ultrasonico_con_puerta_cerrada && 
															sensor_puerta > 0 )
																				{
																				 I2C_perifericos("door","000");
																				}
														}
					}
					//____________________________________________________
			}
			//_______________________________________________________________________________________
			{//-------------------------------------------------------------------------------------- SINCRONIZADOR EVENTOS ENTRE "Sistema Anti Fallas" y "Grower"

					{//----------------------------------------------------------- LECTURA TEMA POR REPRODUCIR
					 I2C_lectura_sensor("play");

					 tema_pedido = String(String(char_mensaje_recibido_del_NodeMcu[5]) +
										  String(char_mensaje_recibido_del_NodeMcu[6]) +
										  String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																{
																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																}
					 if(MP3_BUSY == true)
										{
										 MP3_BUSY = myMP3.isPlaying();
										}

					 if( tema_pedido > 0 && 
						 tema_pedido <= 900 )
										 {
										  myMP3.play(tema_pedido);
										  MP3_BUSY = true;
										 }

					 if( tema_pedido ==  999 && 
						 MP3_BUSY == false )
										 {
										  myMP3.play(puntero_playlist_mp3);
										  MP3_BUSY = true;

										  puntero_playlist_mp3 = puntero_playlist_mp3 + 1;
										  
										  if(puntero_playlist_mp3 <= 250)
																	   {
																		EEPROM.update(addr_0_EEPROM, (byte)(puntero_playlist_mp3));
																	   }
										  if(puntero_playlist_mp3 > 250 && puntero_playlist_mp3 <= 500)
																	   {
																		EEPROM.update(addr_1_EEPROM, (byte)(puntero_playlist_mp3 - 250));
																	   }
										  if(puntero_playlist_mp3 > 500 && puntero_playlist_mp3 <= 750)
																	   {
																		EEPROM.update(addr_2_EEPROM, (byte)(puntero_playlist_mp3 - 500));
																	   }
										  if(puntero_playlist_mp3 > 750 && puntero_playlist_mp3 <= 900)
																	   {
																		EEPROM.update(addr_3_EEPROM, (byte)(puntero_playlist_mp3 - 750));
																	   }
										 }
					 if( tema_pedido ==  987 )
										 {
										  EEPROM.update(addr_0_EEPROM, (byte)(6));
										  EEPROM.update(addr_1_EEPROM, (byte)(0));
										  EEPROM.update(addr_2_EEPROM, (byte)(0));
										  EEPROM.update(addr_3_EEPROM, (byte)(0));

										  puntero_playlist_mp3 = 6;
										 }
					}
					//____________________________________________________________
					{//----------------------------------------------------------- LECTURA DE DATOS PARA CONTROLAR EC + MANTENER NIVEL ESTABLE SOL. NUT. 
							
							{//----------------------------------------- lectura millis_V_EC_mas
							 I2C_lectura_sensor_abrebiado_3_digitos("mef");

							 millis_V_EC_mas = (double)(String(String(char_mensaje_recibido_del_NodeMcu[0]) +
													   String(char_mensaje_recibido_del_NodeMcu[1]) +
													   String(char_mensaje_recibido_del_NodeMcu[2]) +
													   String(char_mensaje_recibido_del_NodeMcu[3]) ).toInt());

							 millis_V_EC_mas = (double)((calibracion_EC_mas_M*millis_V_EC_mas) + calibracion_EC_mas_Y);

							 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																		{
																		 char_mensaje_recibido_del_NodeMcu[i] =' ';
																		}
							}
							//__________________________________________
							{//----------------------------------------- lectura millis_V_EC_menos
							 I2C_lectura_sensor_abrebiado_3_digitos("mer");

							 millis_V_EC_menos = (double)(String(String(char_mensaje_recibido_del_NodeMcu[0]) +
														String(char_mensaje_recibido_del_NodeMcu[1]) +
														String(char_mensaje_recibido_del_NodeMcu[2]) +
														String(char_mensaje_recibido_del_NodeMcu[3]) ).toInt());

							 millis_V_EC_menos = (double)((calibracion_EC_menos_M*millis_V_EC_menos) + calibracion_EC_menos_Y);

							 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																		{
																		 char_mensaje_recibido_del_NodeMcu[i] =' ';
																		}
							}
							//__________________________________________
							{//----------------------------------------- lectura syEC
							 I2C_lectura_sensor("syEC");

							 syEC = String(String(char_mensaje_recibido_del_NodeMcu[5]) +
										   String(char_mensaje_recibido_del_NodeMcu[6]) +
										   String(char_mensaje_recibido_del_NodeMcu[7]) );

							 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																		{
																		 char_mensaje_recibido_del_NodeMcu[i] =' ';
																		}
							}
							//__________________________________________
							{//----------------------------------------- modo Normal
							if(syEC == "OKK")
											{
											 digitalWrite(pin_5v_sensor_nivel_1_RECP_SOL_NUT, HIGH);
											 aux_frecuencia_7 = millis();
											 aux_frecuencia_8 = millis();

											 if(millis_V_EC_mas > 0)
															{
															 INTERRUPTOR("BOMBA_EC_mas", ON);
															}
											 if(millis_V_EC_menos > 0)
															{
															 INTERRUPTOR("BOMBA_EC_menos", ON);
															}

											 while( (digitalRead(pin_sensor_nivel_1_RECP_SOL_NUT) == HIGH) && (aux_terminar_MANTENER_NIVEL_ESTABLE_SOL_NUT == OFF) )
														{
														 if(millis() - aux_frecuencia_7 >= (long)millis_V_EC_menos)
																		{
																		 INTERRUPTOR("BOMBA_EC_menos", OFF);
																		 aux_1_MANTENER_NIVEL_ESTABLE_SOL_NUT = ON;
																		}
														 if(millis() - aux_frecuencia_8 >= (long)millis_V_EC_mas)
																		{
																		 INTERRUPTOR("BOMBA_EC_mas", OFF);
																		 aux_2_MANTENER_NIVEL_ESTABLE_SOL_NUT = ON;
																		}

														if( aux_1_MANTENER_NIVEL_ESTABLE_SOL_NUT == ON && aux_2_MANTENER_NIVEL_ESTABLE_SOL_NUT == ON )
																		{
																		 aux_terminar_MANTENER_NIVEL_ESTABLE_SOL_NUT = ON;
																		}
														}

											 INTERRUPTOR("BOMBA_EC_menos", OFF);
											 INTERRUPTOR("BOMBA_EC_mas", OFF);

											 digitalWrite(pin_5v_sensor_nivel_1_RECP_SOL_NUT, LOW);
											 aux_terminar_MANTENER_NIVEL_ESTABLE_SOL_NUT = OFF;
											 aux_1_MANTENER_NIVEL_ESTABLE_SOL_NUT = OFF;
											 aux_2_MANTENER_NIVEL_ESTABLE_SOL_NUT = OFF;
											 
											 millis_V_EC_menos = 0;
											 millis_V_EC_mas = 0;
											 syEC = "ERR";
											}
							}
							//__________________________________________
							{//----------------------------------------- modo Solo Agua
							if(syEC == "MSA")
											{
											 digitalWrite(pin_5v_sensor_nivel_1_RECP_SOL_NUT, HIGH);

											 while(digitalRead(pin_sensor_nivel_1_RECP_SOL_NUT) == HIGH)
														{
														 INTERRUPTOR("BOMBA_EC_menos", ON);
														}
											 
											 INTERRUPTOR("BOMBA_EC_menos", OFF);
											 
											 digitalWrite(pin_5v_sensor_nivel_1_RECP_SOL_NUT, LOW);
											 syEC = "ERR";
											}
							}
							//__________________________________________
					}
					//____________________________________________________________
/*					{//----------------------------------------------------------- CONSULTA PING --> I2C " TODO OK "
					 if(millis() - aux_frecuencia_6 > 180000)
									{
									 aux_frecuencia_6 = millis();

									 I2C_lectura_sensor("repo");

									 reporte = String(String(char_mensaje_recibido_del_NodeMcu[5]) +
													  String(char_mensaje_recibido_del_NodeMcu[6]) +
													  String(char_mensaje_recibido_del_NodeMcu[7]) );

									 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																				{
																				 char_mensaje_recibido_del_NodeMcu[i] =' ';
																				}
									 if(reporte != "OKK")
														{
														 INTERRUPTOR("REINICIO_TOTAL", 69);
														}
									}
					}
					//____________________________________________________________
*/					{//----------------------------------------------------------- CAMBIAR FRECUENCIA LECTURA SENSORES EC++, EC-- , EC_sol_nut
					 I2C_lectura_sensor("CALI");

					 CALI = String(String(char_mensaje_recibido_del_NodeMcu[5]) +
								   String(char_mensaje_recibido_del_NodeMcu[6]) +
								   String(char_mensaje_recibido_del_NodeMcu[7]) );

					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																{
																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																}
																
					 if( CALI == "999")
										{
											aux_frecuencia_lectura_EC = 2000;
										}
					 else if( CALI == "500")
										{
											aux_frecuencia_lectura_EC = 900000;
										}
					}
					//____________________________________________________________
			}
			//_______________________________________________________________________________________
		}
//########################################################################################################################################################################################################