//######################################################################################################################################################################################################## PRE SETUP
//---------------------------------------------------------------------------------------------------- LIBRERIAS
#include <Wire.h>
#include <RBDdimmer.h>
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- VARIABLES AUXILIARES
#define pwm_OFF 500
#define OFF 0
#define ON 1

unsigned long aux_frecuencia_1 = 0;
unsigned long aux_frecuencia_2 = 0;
unsigned long aux_frecuencia_3 = 0;
unsigned long aux_frecuencia_4 = 0;
unsigned long aux_frecuencia_5 = 0;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION DE LOS PERIFERICOS
#define pin_IN_3_L298N 13
#define pin_IN_4_L298N 12

#define pin_L_PWM_BTS7960 10
#define pin_R_PWM_BTS7960 9
#define pin_LR_EN_BTS7960 8

#define pin_PWM_ACDIMMER 3
#define pin_ZC_ACDIMMER 2

int COMPUERTA_AIRE = pwm_OFF;
int PELTIER_DESHUMIDIFICADOR = pwm_OFF;
int VENTILADOR_PRINCIPAL = pwm_OFF;

int aux_COMPUERTA_AIRE = pwm_OFF;
int aux_PELTIER_DESHUMIDIFICADOR = pwm_OFF;
int aux_VENTILADOR_PRINCIPAL = pwm_OFF;

int millis_apertura_compuerta_aire = 250;
int millis_cierre_compuerta_aire = 125;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION I2C
int I2C_BUSY = OFF;
String mensaje_por_enviar_al_NodeMcu;
const int largo_mensaje_I2C = 8;
char char_mensaje_recibido_del_NodeMcu[largo_mensaje_I2C + 1];
char char_mensaje_por_enviar_al_NodeMcu[largo_mensaje_I2C + 1];
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION SENSOR EC
float RESISTENCIA_sensor_EC_sol_nut = 4700;

#define pin_5v_sensor_EC_sol_nut 7
#define pin_sensor_EC_sol_nut A0
float sensor_PPM_sol_nut = -1;
float raw_sensor_EC_sol_nut = -1;
int int_sensor_PPM_sol_nut = -1;
int buf_EC_sol_nut[10];
int temporal_EC_sol_nut = 0;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION SENSOR NIVEL RECP. PH--
#define pin_5v_sensor_nivel_RECP_PH_menos 5
#define pin_sensor_nivel_RECP_PH_menos A2
int sensor_nivel_RECP_PH_menos = -1;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION SENSOR NIVEL RECP. PH++
#define pin_5v_sensor_nivel_RECP_PH_mas 4
#define pin_sensor_nivel_RECP_PH_mas A3
int sensor_nivel_RECP_PH_mas = -1;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION SENSOR NIVEL RESERVA HUMIDIFICADOR
#define pin_5v_sensor_nivel_extra_inferior 6
#define pin_sensor_nivel_extra_inferior A1
int sensor_nivel_extra_inferior = -1;
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INICIALIZACION AC DIMMER
dimmerLamp dimmer(pin_PWM_ACDIMMER);
//____________________________________________________________________________________________________
//########################################################################################################################################################################################################
//######################################################################################################################################################################################################## FUNCIONES
//----------------------------------------------------------------------------------------------------
void I2C_s__RECEIVE(int howMany)
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
											 }
						}
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- I2C LECTURA SENSOR
void I2C_lectura_sensor(String sensor_abrebiado)
				{
				 mensaje_por_enviar_al_NodeMcu = String(String(sensor_abrebiado) + String(",") + String("LEC"));
				 mensaje_por_enviar_al_NodeMcu.toCharArray(char_mensaje_por_enviar_al_NodeMcu, largo_mensaje_I2C + 1);
				 I2C_BUSY = ON;
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
									 }
				}
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- I2C TODO OK
void I2C_TODO_OK()
				{
				 mensaje_por_enviar_al_NodeMcu = String("okextpue");
				 mensaje_por_enviar_al_NodeMcu.toCharArray(char_mensaje_por_enviar_al_NodeMcu, largo_mensaje_I2C + 1);
				 I2C_BUSY = ON;
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
									 }
				}
//____________________________________________________________________________________________________
//---------------------------------------------------------------------------------------------------- INTERRUPTOR
void INTERRUPTOR(String objeto, int estado)
											{
											 if(objeto == "L298N" && estado != COMPUERTA_AIRE)
																{
																 COMPUERTA_AIRE = estado;
																 if(estado == pwm_OFF)
																				{
																				 digitalWrite (pin_IN_3_L298N, LOW);
																				 digitalWrite (pin_IN_4_L298N, LOW);
																				}
																 else if(estado == 400)
																						{
																						 digitalWrite (pin_IN_3_L298N, HIGH);
																						 digitalWrite (pin_IN_4_L298N, LOW);
																						 delay(millis_cierre_compuerta_aire);
																						 
																						 digitalWrite (pin_IN_3_L298N, LOW);
																						 digitalWrite (pin_IN_4_L298N, LOW);
																						}
																 else if(estado == 600)
																						{
																						 digitalWrite (pin_IN_3_L298N, LOW);
																						 digitalWrite (pin_IN_4_L298N, HIGH);
																						 delay(millis_apertura_compuerta_aire);
																						 
																						 digitalWrite (pin_IN_3_L298N, LOW);
																						 digitalWrite (pin_IN_4_L298N, LOW);
																						}
																}
											 else if(objeto == "BTS7960" && estado != PELTIER_DESHUMIDIFICADOR)
																{
																 PELTIER_DESHUMIDIFICADOR = estado;
																 if(estado == pwm_OFF)
																				{
																				 digitalWrite (pin_L_PWM_BTS7960, LOW);
																				 digitalWrite (pin_R_PWM_BTS7960, LOW);
																				 digitalWrite (pin_LR_EN_BTS7960, LOW);	
																				}
																 else if(estado == 400)
																						{
																						 digitalWrite (pin_L_PWM_BTS7960, HIGH);
																						 digitalWrite (pin_R_PWM_BTS7960, LOW);
																						 digitalWrite (pin_LR_EN_BTS7960, HIGH);	

																						}
																 else if(estado == 600)
																						{
																						 digitalWrite (pin_L_PWM_BTS7960, LOW);
																						 digitalWrite (pin_R_PWM_BTS7960, HIGH);
																						 digitalWrite (pin_LR_EN_BTS7960, HIGH);	
																						}
																}
											 else if(objeto == "AC_DIMMER" && estado != VENTILADOR_PRINCIPAL)
																{
																 VENTILADOR_PRINCIPAL = estado;
																 if(estado == pwm_OFF)
																						{
																						 dimmer.setPower(0);
																						 dimmer.setState(OFF);
																						}
																 else if(estado != pwm_OFF)
																				{
																				 dimmer.setPower( (estado - 500) );
																				 dimmer.setState(ON);
																				}
																}
											 else if(objeto == "TODO" && estado == OFF)
																{
																 COMPUERTA_AIRE = pwm_OFF;
																 PELTIER_DESHUMIDIFICADOR = pwm_OFF;
																 VENTILADOR_PRINCIPAL = pwm_OFF;
																 
																 digitalWrite (pin_IN_3_L298N, LOW);
																 digitalWrite (pin_IN_4_L298N, LOW);

																 digitalWrite (pin_L_PWM_BTS7960, LOW);
																 digitalWrite (pin_R_PWM_BTS7960, LOW);
																 digitalWrite (pin_LR_EN_BTS7960, LOW);

																 dimmer.setPower(0);
																 dimmer.setState(OFF);
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
			 //--------------------------------------------------------------------------------------- PINMODE PARA TODOS LOS PIN'S
			 pinMode(pin_L_PWM_BTS7960, OUTPUT);
			 pinMode(pin_R_PWM_BTS7960, OUTPUT);
			 pinMode(pin_LR_EN_BTS7960, OUTPUT);
			 
			 pinMode(pin_IN_3_L298N, OUTPUT);
			 pinMode(pin_IN_4_L298N, OUTPUT);
			 
			 digitalWrite (pin_L_PWM_BTS7960, LOW);
			 digitalWrite (pin_R_PWM_BTS7960, LOW);
			 digitalWrite (pin_LR_EN_BTS7960, LOW);			 
			 digitalWrite (pin_IN_3_L298N, LOW);
			 digitalWrite (pin_IN_4_L298N, LOW);

			 pinMode(pin_sensor_EC_sol_nut, INPUT);
			 pinMode(pin_sensor_nivel_RECP_PH_menos, INPUT);
			 pinMode(pin_sensor_nivel_RECP_PH_mas, INPUT);
			 pinMode(pin_sensor_nivel_extra_inferior, INPUT);

			 pinMode(pin_5v_sensor_EC_sol_nut, OUTPUT);
			 pinMode(pin_5v_sensor_nivel_RECP_PH_menos, OUTPUT);
			 pinMode(pin_5v_sensor_nivel_RECP_PH_mas, OUTPUT);
			 pinMode(pin_5v_sensor_nivel_extra_inferior, OUTPUT);

			 digitalWrite(pin_5v_sensor_EC_sol_nut, LOW);
			 digitalWrite(pin_5v_sensor_nivel_RECP_PH_menos, LOW);
			 digitalWrite(pin_5v_sensor_nivel_RECP_PH_mas, LOW);
			 digitalWrite(pin_5v_sensor_nivel_extra_inferior, LOW);
			 //_______________________________________________________________________________________
			 //--------------------------------------------------------------------------------------- INICIO AC DIMMER
			 dimmer.begin(NORMAL_MODE, ON);
			 dimmer.setPower(0);
			 dimmer.setState(OFF);
			 //_______________________________________________________________________________________
			 delay(1000);
			 //--------------------------------------------------------------------------------------- INICIO ENLACE I2C
			 Wire.begin(11);
			 //_______________________________________________________________________________________
			 //--------------------------------------------------------------------------------------- INTERRUPTOR ( todo , off )
			 INTERRUPTOR("TODO", OFF);
			 //_______________________________________________________________________________________
			{//--------------------------------------------------------------------------------------- LECTURA SENSOR NIVEL RECP. PH--
			 digitalWrite(pin_5v_sensor_nivel_RECP_PH_menos, HIGH);

			 if(digitalRead(pin_sensor_nivel_RECP_PH_menos) == LOW)
																 {
																  sensor_nivel_RECP_PH_menos = ON;
																  digitalWrite(pin_5v_sensor_nivel_RECP_PH_menos, LOW);
																  I2C_perifericos("rph-","001");
																 }

			 else if (digitalRead(pin_sensor_nivel_RECP_PH_menos) == HIGH)
																 {
																  sensor_nivel_RECP_PH_menos = OFF;
																  digitalWrite(pin_5v_sensor_nivel_RECP_PH_menos, LOW);
																  I2C_perifericos("rph-","000");
																 }
			}
			//________________________________________________________________________________________
			{//--------------------------------------------------------------------------------------- LECTURA SENSOR NIVEL RECP. PH++ 
			 digitalWrite(pin_5v_sensor_nivel_RECP_PH_mas, HIGH);

			 if(digitalRead(pin_sensor_nivel_RECP_PH_mas) == LOW)
																 {
																  sensor_nivel_RECP_PH_mas = ON;
																  digitalWrite(pin_5v_sensor_nivel_RECP_PH_mas, LOW);
																  I2C_perifericos("rph+","001");
																 }

			 else if (digitalRead(pin_sensor_nivel_RECP_PH_mas) == HIGH)
																 {
																  sensor_nivel_RECP_PH_mas = OFF;
																  digitalWrite(pin_5v_sensor_nivel_RECP_PH_mas, LOW);
																  I2C_perifericos("rph+","000");
																 }
			}
			//________________________________________________________________________________________
		}
//########################################################################################################################################################################################################
//######################################################################################################################################################################################################## LOOP
void loop()
		{
			 delay(200);

			{//-------------------------------------------------------------------------------------- PING --> I2C TODO OK
//			 if(millis() - aux_frecuencia_1 > 2000)
//												{
//												 aux_frecuencia_1 = millis();
//												 I2C_TODO_OK();
//												}
			}
			//_______________________________________________________________________________________
			{//-------------------------------------------------------------------------------------- LECTURA DE SENSORES

					{//--------------------------------------------------- LECTURA SENSOR EC
					 if(millis() - aux_frecuencia_2 > 2000)
														{
														 aux_frecuencia_2 = millis();

														 digitalWrite(pin_5v_sensor_EC_sol_nut, HIGH);

														 for(int i=0;i<10;i++) 
																			 { 
																			 buf_EC_sol_nut[i]=analogRead(pin_sensor_EC_sol_nut);
																			 delay(100);
																			 }
														 for( int i = 0 ; i < 9 ; i++ )
																					 {
																					 for( int j = i + 1 ; j < 10 ; j++ )
																														{
																														 if( buf_EC_sol_nut[i] > buf_EC_sol_nut[j] )
																																			 {
																																			 temporal_EC_sol_nut=buf_EC_sol_nut[i];
																																			 buf_EC_sol_nut[i]=buf_EC_sol_nut[j];
																																			 buf_EC_sol_nut[j]=temporal_EC_sol_nut;
																																			 }
																														}
																					 }

														 raw_sensor_EC_sol_nut = 0;
														 raw_sensor_EC_sol_nut = buf_EC_sol_nut[2] + buf_EC_sol_nut[3] + buf_EC_sol_nut[4] + buf_EC_sol_nut[5] + buf_EC_sol_nut[6] + buf_EC_sol_nut[7]; 
														 raw_sensor_EC_sol_nut = raw_sensor_EC_sol_nut/6;

														 digitalWrite(pin_5v_sensor_EC_sol_nut, LOW);

														 sensor_PPM_sol_nut = ((((1000/((((((5*raw_sensor_EC_sol_nut)/1024.0)*(RESISTENCIA_sensor_EC_sol_nut+25))/(5-((5*raw_sensor_EC_sol_nut)/1024.0)))-25)))))*500);
														 
														 int_sensor_PPM_sol_nut = (int)(sensor_PPM_sol_nut); // NO compensado por temperatura

														 if(int_sensor_PPM_sol_nut >= 100 && int_sensor_PPM_sol_nut < 1000)
																			 {
																			  I2C_perifericos("ecsn", String(int_sensor_PPM_sol_nut) );
																			 }
														 if(int_sensor_PPM_sol_nut >= 10 && int_sensor_PPM_sol_nut < 100)
																			 {
																			  I2C_perifericos("ecsn", String( String("0") + String(int_sensor_PPM_sol_nut) ));
																			 }
														 if(int_sensor_PPM_sol_nut < 10)
																			 {
																			  I2C_perifericos("ecsn", String( String("00") + String(int_sensor_PPM_sol_nut) ));
																			 }
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA SENSOR NIVEL RECP. PH--
					 if(millis() - aux_frecuencia_3 > 3600000)
														{
														 aux_frecuencia_3 = millis();

														 digitalWrite(pin_5v_sensor_nivel_RECP_PH_menos, HIGH);

														 if(digitalRead(pin_sensor_nivel_RECP_PH_menos) == LOW)
																											 {
																											  sensor_nivel_RECP_PH_menos = ON;
																											  digitalWrite(pin_5v_sensor_nivel_RECP_PH_menos, LOW);
																											  I2C_perifericos("rph-","001");
																											 }

														 else if (digitalRead(pin_sensor_nivel_RECP_PH_menos) == HIGH)
																											 {
																											  sensor_nivel_RECP_PH_menos = OFF;
																											  digitalWrite(pin_5v_sensor_nivel_RECP_PH_menos, LOW);
																											  I2C_perifericos("rph-","000");
																											 }
														}
					}
					//____________________________________________________
					{//--------------------------------------------------- LECTURA SENSOR NIVEL RECP. PH++
					 if(millis() - aux_frecuencia_4 > 3600000)
														{
														 aux_frecuencia_4 = millis();

														 digitalWrite(pin_5v_sensor_nivel_RECP_PH_mas, HIGH);

														 if(digitalRead(pin_sensor_nivel_RECP_PH_mas) == LOW)
																											 {
																											  sensor_nivel_RECP_PH_mas = ON;
																											  digitalWrite(pin_5v_sensor_nivel_RECP_PH_mas, LOW);
																											  I2C_perifericos("rph+","001");
																											 }

														 else if (digitalRead(pin_sensor_nivel_RECP_PH_mas) == HIGH)
																											 {
																											  sensor_nivel_RECP_PH_mas = OFF;
																											  digitalWrite(pin_5v_sensor_nivel_RECP_PH_mas, LOW);
																											  I2C_perifericos("rph+","000");
																											 }
														}
					}
					//____________________________________________________
			}
			//_______________________________________________________________________________________
			{//-------------------------------------------------------------------------------------- SINCRONIZADOR EVENTOS ENTRE "Extension Puertos" y "Grower"

					{//----------------------------------------------------------- LECTURA VALOR AC DIMMER
					 I2C_lectura_sensor("VEPR");

					 aux_VENTILADOR_PRINCIPAL = String(String(char_mensaje_recibido_del_NodeMcu[5]) +
												   String(char_mensaje_recibido_del_NodeMcu[6]) +
												   String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																{
																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																}

					 if(aux_VENTILADOR_PRINCIPAL == pwm_OFF || aux_VENTILADOR_PRINCIPAL == 0)
											{
											 INTERRUPTOR("AC_DIMMER", pwm_OFF);
											}

					 else if(aux_VENTILADOR_PRINCIPAL != pwm_OFF && aux_VENTILADOR_PRINCIPAL != 0)
											{
											 INTERRUPTOR("AC_DIMMER", aux_VENTILADOR_PRINCIPAL);
											}
					}
					//____________________________________________________________
					{//----------------------------------------------------------- LECTURA VALOR BTS7960
					 I2C_lectura_sensor("PDHU");

					 aux_PELTIER_DESHUMIDIFICADOR = String(String(char_mensaje_recibido_del_NodeMcu[5]) +
													   String(char_mensaje_recibido_del_NodeMcu[6]) +
													   String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																{
																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																}

					 if(aux_PELTIER_DESHUMIDIFICADOR == pwm_OFF || aux_PELTIER_DESHUMIDIFICADOR == 0)
											{
											 INTERRUPTOR("BTS7960", pwm_OFF);
											}

					 else if(aux_PELTIER_DESHUMIDIFICADOR != pwm_OFF && aux_PELTIER_DESHUMIDIFICADOR != 0)
											{
											 INTERRUPTOR("BTS7960", aux_PELTIER_DESHUMIDIFICADOR);
											}
					}
					//____________________________________________________________
					{//----------------------------------------------------------- LECTURA VALOR L298N
					 I2C_lectura_sensor("COAI");

					 aux_COMPUERTA_AIRE = String(String(char_mensaje_recibido_del_NodeMcu[5]) +
											 String(char_mensaje_recibido_del_NodeMcu[6]) +
											 String(char_mensaje_recibido_del_NodeMcu[7]) ).toInt();

					 for (int i = 0; i < largo_mensaje_I2C + 1; i++)
																{
																 char_mensaje_recibido_del_NodeMcu[i] =' ';
																}

					 if(aux_COMPUERTA_AIRE == pwm_OFF || aux_COMPUERTA_AIRE == 0)
											{
											 INTERRUPTOR("L298N", pwm_OFF);
											}

					 else if(aux_COMPUERTA_AIRE != pwm_OFF && aux_COMPUERTA_AIRE != 0)
											{
											 INTERRUPTOR("L298N", aux_COMPUERTA_AIRE);
											}
					}
					//____________________________________________________________
			}
			//_______________________________________________________________________________________
		}
//########################################################################################################################################################################################################