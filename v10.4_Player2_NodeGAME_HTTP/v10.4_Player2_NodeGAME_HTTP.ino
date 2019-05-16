/*Libraries that have been used in this program*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(3);    //Display Initiation. Don't change!

/*  NodeMCU Button KeyMap
 * 
 * Pin D0 = 16    Pin D5 = 14
 * Pin D1 = 5     Pin D6 = 12
 * Pin D2 = 4     Pin D7 = 13
 * Pin D3 = 0     Pin D8 = 15
 * Pin D4 = 2
 * 
 * I2C OLED Display already using the Pin D1 and Pin D2 for SCL and SDA Pinout respectively.
 * 
 */
   String projectName = "NodeGAME+ v10.4.2";
  String software_version = "v10.4.2";
  String player_ID = "P2";
  
  const int button_left = 12;      
  const int button_right = 14;     
  const int button_up = 2;
  const int button_down = 13;
  const int button_enter = 0;

  int up; int down; int left; int right;  int enter;

/* IMPORTANT -------------> PINGPONG VARIABLE
 *  
 * This is the variable that will be used for PingPong Game.
 * Any changes will seriously change the behaviour of the game.
 * Modify carefully!
 * 
 */
  int resolution_width = 128;
  int resolution_height = 64;    
  
  int ball_coorX = 20;    
  int ball_coorY = 32;    
  
  const int PIXEL_SIZE = 8;
  const int WALL_WIDTH = 4;
  const int PADDLE_WIDTH = 4;
  const int BALL_SIZE = 4;
  const int KECEPATAN = 5;
  
  int playerOne_Score = 0;
  int playerTwo_Score = 0;
  int playerOne_Position = 15;
  int playerTwo_Position = 0;
  int state = 32;
  
  char ballDirectionHori = 'R';
  char ballDirectionVerti = 'S';
  boolean inProgress = true;

  int loading_pingpong = 1;  //0 is FALSE, 1 is TRUE
  
  int p1_ready = 0;
  int counter = 0;

/* IMPORTANT -------------> TICTACTOE VARIABLE
 *  
 * This is the variable that will be used for TicTacToe Game.
 * Any changes will seriously change the behaviour of the game.
 * Modify carefully!
 * 
 */
  int selX = 2;
  int selY = 2;
  int scoreX, scoreO;

  char player = 'X';
  boolean continueGame = true;

  char field[3][3]; //field[X][Y]
  bool fieldx[3][3]; //fieldx[X][Y]
  const int coords[3] = {6, 27, 48};

  int selBoxX; int selBoxY;

  int loading_tictactoe = 1;  //0 is FALSE, 1 is TRUE
  int ticktock = 0;

  
  int X_ready;
  
  int X_row; int X_column;  int X_turn;
  int O_row; int O_column;  int O_turn;

/* IMPORTANT -------------> MQTT CREDENTIAL CORE
 *  
 * This is the variable that will be used for MQTT Connectivity.
 * Any changes will seriously change the behaviour of the Network.
 * Modify carefully!
 * 
 * NodeMCU ESP8266 IP Address for HES STUDIO  --> 192.168.0.36
 * NodeMCU ESP8266 IP Address for testarduino --> 192.168.43.170
 * NodeMCU ESP8266 IP Address for testarduino --> 192.168.0.4
 * 
 */

      //Wifi HP
    const char* ssid = "testarduino";    // Enter SSID here
    const char* password = "hardo123";    // Enter Password here
    const char* server = "192.168.43.170";

//  //Laboratory Wifi
//  const char* ssid = "HES STUDIO";
//  const char* password = "akugaktausihkoktanyaaku";
//  const char* server = "192.168.0.36";

//      // Wifi Kontrakan
//    const char* ssid = "The last kontrakan";    // Enter SSID here
//    const char* password = "bayar10000aja";    // Enter Password here
//    const char* server = "192.168.0.4";


  //ESP8266 Wifi Initiation
  WiFiClient client;
//  PubSubClient client(espClient);
  HTTPClient http;
  
/* IMPORTANT -------------> MENU SYSTEM
 *  
 * This is the variable that will be used for Menu Display.
 * Any changes will seriously change the behaviour of the Menu.
 * Modify carefully!
 * 
 */
 
  int chooser = 0;
  int game_id = 0;
  boolean display_menu = true;

  const unsigned char pingpong_bitmap [] PROGMEM = {
  // 'images, 32x32px
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x03, 0xff, 0xc0, 
  0x00, 0x07, 0xff, 0xe0, 0x00, 0x0f, 0xff, 0xf0, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x3f, 0xff, 0xf8, 
  0x00, 0x3f, 0xff, 0xfc, 0x00, 0x7f, 0xff, 0xfc, 0x00, 0x7f, 0xff, 0xfe, 0x00, 0xff, 0xff, 0xfe, 
  0x00, 0xff, 0xff, 0xfe, 0x00, 0xff, 0xff, 0xfc, 0x01, 0xff, 0xff, 0xfc, 0x00, 0xff, 0xff, 0xfc, 
  0x00, 0xff, 0xff, 0xf8, 0x00, 0xff, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xf0, 0x00, 0xbf, 0xff, 0xa0, 
  0x00, 0xcf, 0xf8, 0x80, 0x00, 0xe7, 0xf3, 0xc0, 0x00, 0xf7, 0xff, 0xe0, 0x03, 0xf3, 0xef, 0xf0, 
  0x03, 0xc0, 0x4f, 0xf0, 0x0f, 0x00, 0x07, 0xf0, 0x1e, 0x00, 0x07, 0xe0, 0x3e, 0x00, 0x03, 0xc0, 
  0x3c, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

  const unsigned char tictactoe_bitmap [] PROGMEM = {
  // 'download, 32x32px
  0x02, 0x03, 0xc0, 0x00, 0xe6, 0x67, 0xe6, 0x00, 0xfe, 0x6e, 0x76, 0x00, 0x3c, 0x6c, 0x36, 0x00, 
  0x3c, 0x6c, 0x36, 0x00, 0x7e, 0x6e, 0x76, 0x00, 0xe6, 0x67, 0xe6, 0x00, 0xc2, 0x63, 0xc6, 0x00, 
  0x00, 0xe1, 0x87, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0x60, 0x06, 0x00, 
  0x00, 0x64, 0x26, 0x00, 0x00, 0x6e, 0x76, 0x00, 0x00, 0x67, 0xe6, 0x00, 0x00, 0x63, 0xc6, 0x00, 
  0x00, 0x63, 0xc6, 0x00, 0x00, 0x67, 0xe6, 0x00, 0x00, 0x6e, 0x76, 0x00, 0x00, 0x64, 0x26, 0x00, 
  0x00, 0x60, 0x06, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0x60, 0x06, 0x10, 
  0x00, 0x60, 0x06, 0x3c, 0x00, 0x60, 0x06, 0x7e, 0x00, 0x60, 0x06, 0xe7, 0x00, 0x60, 0x07, 0xc3, 
  0x00, 0x60, 0x07, 0xc3, 0x00, 0x60, 0x06, 0xe7, 0x00, 0x60, 0x06, 0x7e, 0x00, 0x00, 0x00, 0x3c
};

/* IMPORTANT -------------> MQTT WIFI FUNCTION
 *  
 * This is the function that will be used for setting up the Wifi for MQTT.
 * Any changes will seriously change the behaviour of the Wifi.
 * Modify carefully!
 * 
 * void setup_wifi();
 * void callback_pingpong(String topic, byte* message, unsigned int length);
 * void callback_tictactoe(String topic, byte* message, unsigned int length);
 * void reconnect_pingpong();
 * void reconnect_tictactoe();
 * 
 */

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

/* void callback_pingpong(String topic, byte* message, unsigned int length) {
  String messageTemp;
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  //PINGPONG P2 SUBSCRIBER
  if(topic=="pingpong/p1_ready/in")     {p1_ready = messageTemp.toInt();}
  if(topic=="pingpong/p1_position/in")  {playerOne_Position = messageTemp.toInt();}
  if(topic=="pingpong/p1_score/in")     {playerOne_Score = messageTemp.toInt();}
  if(topic=="pingpong/p2_score/in")     {playerTwo_Score = messageTemp.toInt();}
  if(topic=="pingpong/ball_coorX/in")   {ball_coorX = messageTemp.toInt();}
  if(topic=="pingpong/ball_coorY/in")   {ball_coorY = messageTemp.toInt();}
  Serial.println();
}
*/

/* void callback_tictactoe(String topic, byte* message, unsigned int length) {
  String messageTemp;
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  
  //TICTACTOE O SUBSCRIBER
  if(topic=="tictactoe/X_ready/in")   {X_ready = messageTemp.toInt();}
  if(topic=="tictactoe/X_turn/in")    {X_turn = messageTemp.toInt();}
  if(topic=="tictactoe/X_row/in")     {X_row = messageTemp.toInt();}
  if(topic=="tictactoe/X_column/in")  {X_column = messageTemp.toInt();}
  Serial.println();
}
*/

/* void reconnect_pingpong() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266_1")) {
      Serial.println("connected");

      //PINGPONG SUBSCRIPTION
      client.subscribe("pingpong/p1_ready/in");
      client.subscribe("pingpong/p1_position/in");
      client.subscribe("pingpong/p1_score/in");
      client.subscribe("pingpong/p2_score/in");
      client.subscribe("pingpong/ball_coorX/in");
      client.subscribe("pingpong/ball_coorY/in");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
*/

/* void reconnect_tictactoe() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266_1")) {
      Serial.println("connected");
      
      //TICTACTOE SUBSCRIPTION
      client.subscribe("tictactoe/X_ready/in");
      client.subscribe("tictactoe/X_turn/in");
      client.subscribe("tictactoe/X_row/in");
      client.subscribe("tictactoe/X_column/in");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
*/

/* IMPORTANT -------------> SYSTEM CORE FUNCTION
 *  
 * This is the function that will be used for setting up the whole SYSTEM.
 * Any changes will seriously change the system's behaviour.
 * Modify carefully!
 * 
 * void setup();
 * void displayMenu_List();
 * void displayMenu_Cover();
 * void loop();
 * 
 */
 
void setup()   {
  //Serial and Pin Button Setup
  Serial.begin(115200);
  pinMode(button_left, INPUT_PULLUP);
  pinMode(button_right, INPUT_PULLUP);
  pinMode(button_up, INPUT_PULLUP);
  pinMode(button_down, INPUT_PULLUP);
  pinMode(button_enter, INPUT_PULLUP);

  //Display Setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
  displayMenu_Cover();
}

//void displayMenu_List()  {
//  
//  while (display_menu) {
//    display.clearDisplay();
//    display.setTextSize(1);
//    display.setTextColor(WHITE);
//    display.setCursor(4,0);   display.println(projectName);
//    display.setCursor(114,0); display.println(player_ID);
//    display.fillRect(0,10,128,2, WHITE);
//    
//    display.setCursor(16, 20);  display.println("PingPong Single");
//    display.setCursor(16, 31);  display.println("PingPong Multi");
//    display.setCursor(16, 42);  display.println("TicTacToe Single");
//    display.setCursor(16, 53);  display.println("TicTacToe Multi");
//
//    up = digitalRead(button_up);
//    down = digitalRead(button_down);
//    enter = digitalRead(button_enter);
//
//    if (up == HIGH)   {chooser--;if (chooser < 0) {chooser = 0;}}
//    if (down == HIGH) {chooser++;if (chooser > 4) {chooser = 4;}}
//    
//    switch(chooser)  {
//      case 1 :  //PingPong Singleplayer
//        display.setCursor(0, 20); display.println(">>");
//        player_ID = "P1";
//
//        if (enter == LOW) {
//          game_id = 1;
//          display_menu = false;
//        }
//        break;
//
//      case 2 :  //PingPong Multiplayer
//        display.setCursor(0, 31); display.println(">>");
//        player_ID = "P1";
//
//        if (enter == LOW) {
//          game_id = 2;       
//          setup_wifi();
//          client.setServer(mqtt_server, 1883);
//          client.setCallback(callback_pingpong);
//          display_menu = false; 
//        }
//        break;
//
//      case 3 :  //TicTacToe Singleplayer
//        display.setCursor(0, 42); display.println(">>");
//        player_ID = "X";
//
//        if (enter == LOW) {
//          game_id = 3;
//          randomSeed(analogRead(0));
//          display_menu = false; 
//        }
//        break;
//
//      case 4 :  //TicTacToe Multiplayer
//        display.setCursor(0, 53); display.println(">>");
//        player_ID = "X";
//
//        if (enter == LOW) {
//          game_id = 4;       
//          setup_wifi();
//          client.setServer(mqtt_server, 1883);
//          client.setCallback(callback_tictactoe);
//          display_menu = false; 
//        }
//        break;
//    }
//    display.display();
//  }
//}

void displayMenu_Cover()  {
  
  while(display_menu) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(3,2);
    display.println("1 2 3 4 5");
    display.setCursor(102,0); display.println("49%");
    
    left = digitalRead(button_left);
    right = digitalRead(button_right);
    enter = digitalRead(button_enter);

    if (left == HIGH)   {chooser--;if (chooser < 0) {chooser = 0;}}
    if (right == HIGH) {chooser++;if (chooser > 5) {chooser = 5;}}
    
    switch(chooser)  {
      case 1 :  //About
        display.drawRect(0, 0, 11, 11, WHITE); //frame
        display.setTextSize(2);
        display.setCursor(10,20); display.println("NodeGAME+");
        display.setTextSize(1);
        display.setCursor(43,40); display.println(software_version);
        display.setCursor(40,54); display.println(">>>>>>>");
        break;
        
      case 2 :  //PingPong Singleplayer
        display.drawRect(12, 0, 11, 11, WHITE); //frame
        display.drawBitmap(48,16,pingpong_bitmap,32,32,1);
        display.setCursor(0,54);  display.println("Pingpong Singleplayer");

        if (enter == LOW) {
          game_id = 1;
          display_menu = false;
        }
        break;

      case 3 :  //PingPong Multiplayer
        display.drawRect(24, 0, 11, 11, WHITE); //frame
        display.drawBitmap(48,16,pingpong_bitmap,32,32,1);
        display.setCursor(0,54);  display.println("Pingpong Multiplayer");

        if (enter == LOW) {
          game_id = 2;       
          setup_wifi();
          client.connect(server, 80);
//          client.setServer(mqtt_server, 1883);
//          client.setCallback(callback_pingpong);
          display_menu = false; 
        }
        break;

      case 4 :  //TicTacToe Singleplayer
        display.drawRect(36, 0, 11, 11, WHITE); //frame
        display.drawBitmap(48,16,tictactoe_bitmap,32,32,1);
        display.setCursor(0,54);  display.println("TictactoeSingleplayer");

        if (enter == LOW) {
          game_id = 3;
          randomSeed(analogRead(0));
          display_menu = false; 
        }
        break;

      case 5 :  //TicTacToe Multiplayer
        display.drawRect(48, 0, 11, 11, WHITE); //frame
        display.drawBitmap(48,16,tictactoe_bitmap,32,32,1);
        display.setCursor(0,54);  display.println("Tictactoe Multiplayer");

        if (enter == LOW) {
          game_id = 4;       
          setup_wifi();
          client.connect(server, 80);          
//          client.setServer(mqtt_server, 1883);
//          client.setCallback(callback_tictactoe);
          display_menu = false; 
        }
        break;
    }
    display.display();
  }
}

void loop() {

  switch(game_id) {
    case 1: play_PingPong_Single();   break;
    case 2: play_PingPong_Multi();    break;
    case 3: play_TicTacToe_Single();  break;
    case 4: play_TicTacToe_Multi();   break;    
  }
}

/* IMPORTANT! ----------> PINGPONG 
 * 
 * This is the core of Pingpong game.
 * Any changes below will alter the Pingpong game's behaviour.
 * Modify carefully!
 * 
 * void play_PingPong_Single();
 * void play_PingPong_Multi();
 * void ballMovement();
 * void buttonController_PingPong();
 * void gameResult_PingPong();
 * void moveAi_PingPong();
 * void drawScore();
 * void drawNet();
 * void drawPixel(int posX, int posY, int dimensions);
 * void drawPlayerOnePaddle(int row);
 * void drawPlayerTwoPaddle(int row);
 * void drawBall(int x, int y);
 * 
 */

void play_PingPong_Single()  {
  
  display.clearDisplay();
  if (playerTwo_Score == 7 || playerOne_Score == 7) {inProgress = false;}

  if (inProgress) {
    ballMovement();
    buttonController_PingPong();
    drawBall(ball_coorX, ball_coorY);
    drawPlayerTwoPaddle(playerTwo_Position);
    moveAi_PingPong();
    drawNet();
    drawScore();
  } else { // somebody has won
    gameResult_PingPong();
  }
  display.display();
}

void play_PingPong_Multi()  { 

//  if (!client.connected()) {reconnect_pingpong();}
//  if(!client.loop())
//    client.connect("ESP8266_2");

  //This If-Statement make sure that the player's signal ready-to-play is published for once.
  counter = counter + 1;  
  String p2_readyStr;
  int p2_ready = 1;
    
  p2_readyStr = String(p2_ready);
//  static char p2_ready[5];
//    dtostrf(loading_pingpong, 1, 1, p2_ready);
//    client.publish("pingpong/p2_ready/out", p2_ready);
  if (counter < 15){
//  http.begin("http://192.168.0.36:1880/pingpong/p2/post/signal");
  http.begin("http://192.168.43.170:1880/pingpong/p2/post/signal");
//  http.begin("http://192.168.0.4:1880/pingpong/p2/post/signal");
  http.addHeader("Content-Type", "text/plain");
    
  int p2_sendReady = http.POST(p2_readyStr);
  String p2_payloadready = http.getString();
  Serial.println("string payload: " + p2_payloadready);
  http.end();
  }
  //////////////////////PINGPONG GET HTTP REQUEST///////////////////////
  if (p1_ready == 0){
//    http.begin("http://192.168.0.36:1880/pingpong/p1/get/signal");
    http.begin("http://192.168.43.170:1880/pingpong/p1/get/signal");
//    http.begin("http://192.168.0.4:1880/pingpong/p1/get/signal");
    
    int p1_signalGet = http.GET();
    String p1_payloadSignal = http.getString();
    p1_ready = p1_payloadSignal.toInt();
    http.end();
  }

////////////////////////////////////////////////////////////////////////
//    http.begin("http://192.168.0.36:1880/pingpong/p1/get/score");
    http.begin("http://192.168.43.170:1880/pingpong/p1/get/score");
//    http.begin("http://192.168.0.4:1880/pingpong/p1/get/score");
    
    int p1_scoreGet = http.GET();
    String p1_payloadScore = http.getString();
    playerOne_Score = p1_payloadScore.toInt();
    http.end();  
////////////////////////////////////////////////////////////////////////
//    http.begin("http://192.168.0.36:1880/pingpong/p2/get/score");
    http.begin("http://192.168.43.170:1880/pingpong/p2/get/score");
//    http.begin("http://192.168.0.4:1880/pingpong/p2/get/score");
    
    int p2_ScoreGet = http.GET();
    String p2_payloadScore = http.getString();
    playerTwo_Score = p2_payloadScore.toInt();
    http.end();  



  display.clearDisplay();
  if (playerTwo_Score == 5 || playerOne_Score == 5) {
    inProgress = false;
  }

  if (p1_ready == 1) {
    if (inProgress) {

      /*TERMINAL MONITORING*/
//      Serial.print("p1_ready : ");  Serial.println(p1_ready);
//      Serial.print("p2_ready : ");  Serial.println(p2_ready);
//      Serial.println("");
//    
//      Serial.print("P1 : ");  Serial.println(playerOne_Score);
//      Serial.print("P2 : ");  Serial.println(playerTwo_Score);
//      Serial.println("");
//      
//      Serial.print("playerOne_Position : ");  Serial.println(playerOne_Position);
//      Serial.print("playerTwo_Position : ");  Serial.println(playerTwo_Position);
//      Serial.println("");
//    
//      Serial.print("ball_coorX : ");  Serial.println(ball_coorX);
//      Serial.print("ball_coorY : ");  Serial.println(ball_coorY);
//      Serial.println("");
//
      buttonController_PingPong();

//      static char p2_position[3];
//      dtostrf(playerTwo_Position, 1, 1, p2_position);
//      client.publish("pingpong/p2_position/out", p2_position);

      
      drawBall(ball_coorX, ball_coorY);
////////////////////////////////////////////////////////////////////////
//    http.begin("http://192.168.0.36:1880/pingpong/ball/get/coor_x");
    http.begin("http://192.168.43.170:1880/pingpong/ball/get/coor_x");
//    http.begin("http://192.168.0.4:1880/pingpong/ball/get/coor_x");
    
    int ball_coorXGet = http.GET();
    String ball_payloadcoorX = http.getString();
    ball_coorX = ball_payloadcoorX.toInt();
    http.end();  
////////////////////////////////////////////////////////////////////////
//    http.begin("http://192.168.0.36:1880/pingpong/ball/get/coor_y");
    http.begin("http://192.168.43.170:1880/pingpong/ball/get/coor_y");
//    http.begin("http://192.168.0.4:1880/pingpong/ball/get/coor_y");
    
    int ball_coorYGet = http.GET();
    String ball_payloadcoorY = http.getString();
    ball_coorY = ball_payloadcoorY.toInt();
    http.end();  
///////////////////////////////////////////////////////////////////////
      drawPlayerOnePaddle(playerOne_Position);
      drawPlayerTwoPaddle(playerTwo_Position);
////////////////////////////////////////////////////////////////////////
//    http.begin("http://192.168.0.36:1880/pingpong/p1/get/position");
      http.begin("http://192.168.43.170:1880/pingpong/p1/get/position");
//      http.begin("http://192.168.0.4:1880/pingpong/p1/get/position");
    
    int p1_positionGet = http.GET();
    String p1_payloadPosition = http.getString();
    playerOne_Position = p1_payloadPosition.toInt();
    http.end();  
//////////////////////////////////////////////////////////////////////////
      drawNet();
      drawScore();
//////////////////////////////////////////////////////////////////////////
        String postp2_position;
        
        postp2_position = String(playerTwo_Position);
        
//        http.begin("http://192.168.0.36:1880/pingpong/p2/post/position");
        http.begin("http://192.168.43.170:1880/pingpong/p2/post/position");
//        http.begin("http://192.168.0.4:1880/pingpong/p2/post/position");

        http.addHeader("Content-Type", "text/plain");
        
        int p2_sendPosition = http.POST(postp2_position);
        String p2_payloadPosition = http.getString();
        http.end();
//////////////////////////////////////////////////////////////////////////
    } else { // somebody has won
      gameResult_PingPong();
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(14, 28);
      display.println("gg.gg/http_qoe");
    
      display.display();
      delay(60000);
    }
  } else {
    Serial.println("WAITING FOR PLAYER 1 !");
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(20, 16);
    display.println("WAIT FOR   PLAYER 1");
  }
  display.display();
}

void ballMovement() {
  
      if (ballDirectionVerti == 'U') {
        ball_coorY = ball_coorY - KECEPATAN;    // move ball up diagonally
      }
  
      if (ballDirectionVerti == 'D') {
        ball_coorY = ball_coorY + KECEPATAN;    // move ball down diagonally
      }
  
      if (ball_coorY <= 0) {
        ballDirectionVerti = 'D';         // bounce the ball off the top
      }
  
      if (ball_coorY >= resolution_height) {
        ballDirectionVerti = 'U';         // bounce the ball off the bottom
      }
  
      if (ballDirectionHori == 'R') {
        ball_coorX = ball_coorX + KECEPATAN;    // move ball
  
        if (ball_coorX >= (resolution_width - 6)) {
          if ((playerTwo_Position + 12) >= ball_coorY && (playerTwo_Position - 12) <= ball_coorY) {     // ball is at the AI edge of the screen
            if (ball_coorY > (playerTwo_Position + 4)) {
              ballDirectionVerti = 'D';   // deflect ball down
            }
            else if (ball_coorY < (playerTwo_Position - 4)) {
              ballDirectionVerti = 'U';   // deflect ball up
            }
            else {
              ballDirectionVerti = 'S';   // deflect ball straight
            }
            ballDirectionHori = 'L';      // change ball direction
          }
          else {   // GOAL!
            ball_coorX = 6;                  // move ball to other side of screen
            ballDirectionVerti = 'S';     // reset ball to straight travel
            ball_coorY = resolution_height / 2;  // move ball to middle of screen
            ++playerOne_Score;                // increase player score
          }
        }
      }
  
      if (ballDirectionHori == 'L') {
        ball_coorX = ball_coorX - KECEPATAN;            // move ball
  
        if (ball_coorX <= 6) {
          if ((playerOne_Position + 12) >= ball_coorY && (playerOne_Position - 12) <= ball_coorY) {   // ball is at the player edge of the screen
            if (ball_coorY > (playerOne_Position + 4)) {      // ball hits player paddle
              ballDirectionVerti = 'D';           // deflect ball down
            }
            else if (ball_coorY < (playerOne_Position - 4)) {
              ballDirectionVerti = 'U';           // deflect ball up
            }
            else {
              ballDirectionVerti = 'S';         // deflect ball straight
            }
            ballDirectionHori = 'R';              // change ball direction
  
          } else {
            ball_coorX = resolution_width - 6;        // move ball to other side of screen
            ballDirectionVerti = 'S';           // reset ball to straight travel
            ball_coorY = resolution_height / 2;        // move ball to middle of screen
            ++playerTwo_Score;                          // increase AI score
          }
        }
      }
}

void buttonController_PingPong() {
  up = digitalRead(button_up);
  down = digitalRead(button_down);

  if (up == HIGH) {
    if (state != 6) {state = state - (4 * up);}
  }

  if (down == HIGH) {
    if (state != 54) {state = state + (4 * down);}
  }
  playerTwo_Position = state;
}

void gameResult_PingPong() {

  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  
  if (playerTwo_Score > playerOne_Score)      {display.println("YOU  WIN!");}
  else if (playerOne_Score > playerTwo_Score) {display.println("YOU  LOSE!");}
  
  delay(500);
  display.display();
  delay(4000);
}

void moveAi_PingPong() {   // move the AI paddle
  
  if (ball_coorY > playerOne_Position)        {playerOne_Position += 2;}
  else if (ball_coorY < playerOne_Position)   {playerOne_Position -= 2;}
  
  drawPlayerOnePaddle(playerOne_Position);
}

void drawScore() {    // draw AI and player scores

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(45, 0);
  display.println(playerOne_Score);
  display.setCursor(75, 0);
  display.println(playerTwo_Score);
}

void drawNet() {

  for (int i = 0; i < (resolution_height / WALL_WIDTH); ++i) {
    drawPixel(((resolution_width / 2) - 1), i * (WALL_WIDTH) + (WALL_WIDTH * i), WALL_WIDTH);
  }
}

void drawPixel(int posX, int posY, int dimensions) {      // draw group of pixels

  for (int x = 0; x < dimensions; ++x) {
    for (int y = 0; y < dimensions; ++y) {
      display.drawPixel((posX + x), (posY + y), WHITE);
    }
  }
}

void drawPlayerOnePaddle(int row) {

  drawPixel(0, row - (PADDLE_WIDTH * 2), PADDLE_WIDTH);
  drawPixel(0, row - PADDLE_WIDTH, PADDLE_WIDTH);
  drawPixel(0, row, PADDLE_WIDTH);
  drawPixel(0, row + PADDLE_WIDTH, PADDLE_WIDTH);
  drawPixel(0, row + (PADDLE_WIDTH + 2), PADDLE_WIDTH);
}

void drawPlayerTwoPaddle(int row) {

  int column = resolution_width - PADDLE_WIDTH;
  drawPixel(column, row - (PADDLE_WIDTH * 2), PADDLE_WIDTH);  
  drawPixel(column, row - PADDLE_WIDTH, PADDLE_WIDTH);
  drawPixel(column, row, PADDLE_WIDTH);
  drawPixel(column, row + PADDLE_WIDTH, PADDLE_WIDTH);
  drawPixel(column, row + (PADDLE_WIDTH * 2), PADDLE_WIDTH);
}

void drawBall(int x, int y) {
  display.drawCircle(x, y, BALL_SIZE, WHITE);
}

/* IMPORTANT! ----------> TICTACTOE 
 * 
 * This is the core of TicTacToe game.
 * Any changes below will alter the TicTacToe game's behaviour.
 * Modify carefully!
 * 
 * void play_TicTacToe_Single();
 * void play_TicTacToe_Multi();
 * void drawCross(int x, int y);
 * void drawCircle(int x, int y);
 * void playerSwitch();
 * void drawGame();
 * void buttonController_TicTacToe();
 * void moveAi_TicTacToe();
 * void assignElements_single();
 * void assignElements_multi();
 * void checkWinning();
 * void calculateScore(char player);
 * void gameResult_TicTacToe();
 */

void play_TicTacToe_Single() {
  
  display.clearDisplay();
  if((scoreX == 7) || (scoreO == 7))  {continueGame = false;}
  
  if(continueGame == true)  {
    drawGame();
    buttonController_TicTacToe();
    assignElements_single();
    checkWinning();
    display.display();
  }
  else  { 
    gameResult_TicTacToe();
  }
}

void play_TicTacToe_Multi() {

//  if (!client.connected()) {reconnect_tictactoe();}
//  if(!client.loop())
//    client.connect("ESP8266_2");

  //This If-Statement make sure that the player's signal ready-to-play is published for once.
  String O_readyStr;
  int O_ready = 1;
    
  O_readyStr = String(O_ready);
//  static char O_ready[3];
  ticktock = ticktock + 1;
  
  if (ticktock < 15) {
//    dtostrf(loading_tictactoe, 1, 1, O_ready);
//    client.publish("tictactoe/O_ready/out", O_ready);
//    http.begin("http://192.168.0.36:1880/tictactoe/player_o/post/signal");
    http.begin("http://192.168.43.170:1880/tictactoe/player_o/post/signal");
//    http.begin("http://192.168.0.4:1880/tictactoe/player_o/post/signal");
    http.addHeader("Content-Type", "text/plain");

    int O_sendReady = http.POST(O_readyStr);
    String O_payloadready = http.getString();
    Serial.println("string payload ready: " + O_payloadready);
    http.end(); 
  }
  
////////////////////////////////////////////////////////////////////////
//  http.begin("http://192.168.0.36:1880/tictactoe/player_x/get/signal");
  http.begin("http://192.168.43.170:1880/tictactoe/player_x/get/signal");
//  http.begin("http://192.168.0.4:1880/tictactoe/player_x/get/signal");

  
  int pX_signalGet = http.GET();
  String pX_payloadSignal = http.getString();
  X_ready = pX_payloadSignal.toInt();
  http.end();
////////////////////////////////////////////////////////////////////////
/////////////////////TICTACTOE GET HTTP REQUEST///////////////////////
//  http.begin("http://192.168.0.36:1880/tictactoe/player_x/get/turn");
  http.begin("http://192.168.43.170:1880/tictactoe/player_x/get/turn");
//  http.begin("http://192.168.0.4:1880/tictactoe/player_x/get/turn");

  
  int pX_turnGet = http.GET();
  String pX_payloadTurn = http.getString();
  X_turn = pX_payloadTurn.toInt();
  http.end();
////////////////////////////////////////////////////////////////////////
//  http.begin("http://192.168.0.36:1880/tictactoe/player_x/get/row");
  http.begin("http://192.168.43.170:1880/tictactoe/player_x/get/row");
//  http.begin("http://192.168.0.4:1880/tictactoe/player_x/get/row");

  
  int pX_rowGet = http.GET();
  String pX_payloadRow = http.getString();
  X_row = pX_payloadRow.toInt();
  http.end();
////////////////////////////////////////////////////////////////////////
//  http.begin("http://192.168.0.36:1880/tictactoe/player_x/get/column");
  http.begin("http://192.168.43.170:1880/tictactoe/player_x/get/column");
//  http.begin("http://192.168.0.4:1880/tictactoe/player_x/get/column");

  int pX_columnGet = http.GET();
  String pX_payloadColumn = http.getString();
  X_column = pX_payloadColumn.toInt();
  http.end();
////////////////////////////////////////////////////////////////////////

  display.clearDisplay();
  if((scoreX == 5) || (scoreO == 5))  {continueGame = false;}

  if(X_ready == 1)  {
    if(continueGame == true)  {

      drawGame();
      buttonController_TicTacToe();
      assignElements_multi();
      checkWinning();     
      display.display();
    }
    else  { 
      gameResult_TicTacToe();
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 28);
      display.println("gg.gg/http_qoe");
      display.display();
      delay(60000);
    }
  } else {
    Serial.println("WAITING FOR PLAYER X!");
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(20, 16);
    display.println("WAIT FOR   PLAYER X");
    display.display();
  }
}

void drawCross(int x, int y) {
  display.drawLine(x, y, x + 9, y + 9, WHITE); //top left - down right slash
  display.drawLine(x + 9, y, x, y + 9, WHITE); //down left - top right slash
}

void drawCircle(int x, int y) {
  display.drawCircle(x + 4, y + 5, 5, WHITE);
}

void playerSwitch() {
  if (player == 'O')      {player = 'X';}
  else if (player == 'X') {player = 'O';}
}

void drawGame() {

  display.drawRect(0, 0, 64, 64, WHITE); //frame
  display.drawLine(0, 21, 63, 21, WHITE); //horizontal line
  display.drawLine(0, 42, 63, 42, WHITE); //horizontal line
  display.drawLine(21, 0, 21, 63, WHITE); //vertical line
  display.drawLine(42, 0, 42, 63, WHITE); //vertical line

  //draws the game field elements (X or O)
  for (int i = 0; i <= 2; i++) {
    for (int j = 0; j <= 2; j++) {
      if (field[i][j] == 'O')       {drawCircle(coords[i], coords[j]);}
      else if (field[i][j] == 'X')  {drawCross(coords[i], coords[j]);}
    }
  }
    
  //draws the selection box
  int selBoxX, selBoxY;
  if (selX == 1)      {selBoxX = 1;}
  else if (selX == 2) {selBoxX = 22;}
  else if (selX == 3) {selBoxX = 43;}
  
  if (selY == 1){selBoxY = 1;}
  else if (selY == 2) {selBoxY = 22;}
  else if (selY == 3) {selBoxY = 43;}
    
  display.drawRect(selBoxX, selBoxY, 20, 20, WHITE);
  display.drawRect(selBoxX+1, selBoxY+1, 18, 18, WHITE);

  //draws the sidebar
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(81, 0);   display.println("SCORE");
  display.setCursor(75, 14);  display.println("X:");
  display.setCursor(88, 14);  display.print(scoreX);
  display.setCursor(75, 25);  display.println("O:");
  display.setCursor(88, 25);  display.print(scoreO);
  display.setCursor(75, 36);  display.println("Player:");
       
  if (player == 'X')      {drawCross(93, 50);}
  else if (player == 'O') {drawCircle(93 , 50);}
}
 
void buttonController_TicTacToe() {

  //assign controller to certain variable
  left = digitalRead(button_left);
  right = digitalRead(button_right);
  up = digitalRead(button_up);     
  down = digitalRead(button_down);
  enter = digitalRead(button_enter);

//  Serial.print("left  : ");   Serial.println(left);
//  Serial.print("right : ");   Serial.println(right);
//  Serial.print("up    : ");   Serial.println(up);
//  Serial.print("down  : ");   Serial.println(down);  
//  Serial.print("enter : ");   Serial.println(enter);

  //Controlling overflow in button
  if (left == HIGH)   {selX--;}                                 
  if (right == HIGH)  {selX++;}
  if (up == HIGH)     {selY--;}                                 
  if (down == HIGH)   {selY++;}

  //Limiting the cellbox if over or under certain number
  if (selX > 3)       {selX = 3;} 
  else if (selX < 1)  {selX = 1;}
  
  if (selY > 3)       {selY = 3;}
  else if (selY < 1)  {selY = 1;}
}

void moveAi_TicTacToe() {

  do {
    O_row = random(0,2);
    O_column = random(0,2);
  } while (fieldx[O_row][O_column] == true);
}

void assignElements_single() {
  //assigns the game field elements their value (X or O) when the joystick button is pressed
  if (player == 'X') {
    for (int i = 0; i <= 2; i++) {
      for (int j = 0; j <= 2; j++) {
        if ((selX == i+1) && (selY == j+1) && (fieldx[i][j] == false)) {
          if (enter == LOW) {
            field[i][j] = 'X';
            fieldx[i][j] = true;
            playerSwitch();
          }
        }
      }
    }          
  } else if (player == 'O') {
      moveAi_TicTacToe();
      field[O_row][O_column] = 'O';
      fieldx[O_row][O_column] = true;
      playerSwitch();
  }
}

void assignElements_multi() {
  /*TERMINAL MONITORING*/
//  Serial.print("X_turn: "); Serial.println(X_turn);
//  Serial.print("O_turn: "); Serial.println(O_turn);
//  Serial.println("");
//
//  Serial.print("Player: "); Serial.println(player);
//  Serial.println("");
  
  //assigns the game field elements their value (X or O) when the joystick button is pressed
  if (player == 'O') {
    for (int i = 0; i <= 2; i++) {
      for (int j = 0; j <= 2; j++) {
        if ((selX == i+1) && (selY == j+1) && (fieldx[i][j] == false)) {
          if (enter == LOW) {
            field[i][j] = 'O';
            fieldx[i][j] = true;
  
            O_row = i;
            O_column = j;
            O_turn = 1;
///////////////////////////////////////////////////////////////////////////////  
//            static char Orow[1];
//            dtostrf(O_row, 0, 0, Orow);
//            client.publish("tictactoe/O_row/out", Orow);
            String O_rowStr;
            O_rowStr = String(O_row);
            
//            http.begin("http://192.168.0.36:1880/tictactoe/player_o/post/row");
            http.begin("http://192.168.43.170:1880/tictactoe/player_o/post/row");
//            http.begin("http://192.168.0.4:1880/tictactoe/player_o/post/row");

            http.addHeader("Content-Type", "text/plain");
            
            int O_sendRow = http.POST(O_rowStr);
            String O_payloadRow = http.getString();
            Serial.println("string payload row: " + O_payloadRow);
            http.end();   
////////////////////////////////////////////////////////////////////////////////
//            static char Ocolumn[1];
//            dtostrf(O_column, 0, 0, Ocolumn);
//            client.publish("tictactoe/O_column/out", Ocolumn);
            String O_columnStr;
            
            O_columnStr = String(O_column);
                        
//            http.begin("http://192.168.0.36:1880/tictactoe/player_o/post/column");
            http.begin("http://192.168.43.170:1880/tictactoe/player_o/post/column");
//            http.begin("http://192.168.0.4:1880/tictactoe/player_o/post/column");

            http.addHeader("Content-Type", "text/plain");
            
            int O_sendColumn = http.POST(O_columnStr);
            String O_payloadColumn = http.getString();
            Serial.println("string payload column: " + O_payloadColumn);
            http.end();  
///////////////////////////////////////////////////////////////////////////////
//            static char Oturn[1];
//            dtostrf(O_turn, 0, 0, Oturn);
//            client.publish("tictactoe/O_turn/out", Oturn);
            String O_turnStr;

            O_turnStr = String(O_turn);

//            http.begin("http://192.168.0.36:1880/tictactoe/player_o/post/turn");
            http.begin("http://192.168.43.170:1880/tictactoe/player_o/post/turn");
//            http.begin("http://192.168.0.4:1880/tictactoe/player_o/post/turn");

            http.addHeader("Content-Type", "text/plain");
            
            int O_sendTurn = http.POST(O_turnStr);
            String O_payloadTurn = http.getString();
            Serial.println("string payload: " + O_payloadTurn);
            http.end();  
///////////////////////////////////////////////////////////////////////////////
            delay(1300);
            O_turn = 0;
//            dtostrf(O_turn, 0, 0, Oturn);
//            client.publish("tictactoe/O_turn/out", Oturn);
            O_turnStr = String(O_turn);

//            http.begin("http://192.168.0.36:1880/tictactoe/player_o/post/turn");
            http.begin("http://192.168.43.170:1880/tictactoe/player_o/post/turn");
//            http.begin("http://192.168.0.4:1880/tictactoe/player_o/post/turn");

            http.addHeader("Content-Type", "text/plain");
            
            O_sendTurn = http.POST(O_turnStr);
            O_payloadTurn = http.getString();
            Serial.println("string payload: " + O_payloadTurn);
            http.end(); 
///////////////////////////////////////////////////////////////////////////////
            playerSwitch();
          }
        }
      }
    }          
  } else if (player == 'X') {
      if (X_turn == 1) {
        field[X_row][X_column] = 'X';
        fieldx[X_row][X_column] = true;
        playerSwitch();
      } else if (X_turn == 0) {
        Serial.println("WAITING FOR PLAYER X TO FINISH THE MOVE");
      }
  }
}

void checkWinning() {
  //checks the game field for winning combinations
  if ((field[0][0] == 'X') && (field[1][0] == 'X') && (field[2][0] == 'X')){calculateScore('X');}
  else if ((field[0][1] == 'X') && (field[1][1] == 'X') && (field[2][1] == 'X')){calculateScore('X');}
  else if ((field[0][2] == 'X') && (field[1][2] == 'X') && (field[2][2] == 'X')){calculateScore('X');}
  else if ((field[0][0] == 'X') && (field[0][1] == 'X') && (field[0][2] == 'X')){calculateScore('X');}
  else if ((field[1][0] == 'X') && (field[1][1] == 'X') && (field[1][2] == 'X')){calculateScore('X');}
  else if ((field[2][0] == 'X') && (field[2][1] == 'X') && (field[2][2] == 'X')){calculateScore('X');}
  else if ((field[0][0] == 'X') && (field[1][1] == 'X') && (field[2][2] == 'X')){calculateScore('X');}
  else if ((field[2][0] == 'X') && (field[1][1] == 'X') && (field[0][2] == 'X')){calculateScore('X');}
  else if ((field[0][0] == 'O') && (field[1][0] == 'O') && (field[2][0] == 'O')){calculateScore('O');}
  else if ((field[0][1] == 'O') && (field[1][1] == 'O') && (field[2][1] == 'O')){calculateScore('O');}
  else if ((field[0][2] == 'O') && (field[1][2] == 'O') && (field[2][2] == 'O')){calculateScore('O');}
  else if ((field[0][0] == 'O') && (field[0][1] == 'O') && (field[0][2] == 'O')){calculateScore('O');}
  else if ((field[1][0] == 'O') && (field[1][1] == 'O') && (field[1][2] == 'O')){calculateScore('O');}
  else if ((field[2][0] == 'O') && (field[2][1] == 'O') && (field[2][2] == 'O')){calculateScore('O');}
  else if ((field[0][0] == 'O') && (field[1][1] == 'O') && (field[2][2] == 'O')){calculateScore('O');}
  else if ((field[2][0] == 'O') && (field[1][1] == 'O') && (field[0][2] == 'O')){calculateScore('O');}
  
  else if ((fieldx[0][0] == true) && (fieldx[0][1] == true) && (fieldx[0][2] == true) && (fieldx[1][0] == true) && (fieldx[1][1] == true) && (fieldx[1][2] == true) && (fieldx[2][0] == true) && (fieldx[2][1] == true) && (fieldx[2][2] == true)){calculateScore('T');}
}

void calculateScore(char player) {
  
  if (player == 'X')      {scoreX++;} 
  else if (player == 'O') {scoreO++;}
  else if (player == 'T') {scoreX++;  scoreO++;}
  
  playerSwitch(); //switches the players

  //clears the game field arrays
  for (int i = 0; i <= 2; i++) {
    for (int j = 0; j <= 2; j++) {
       field[i][j] = ' ';
       fieldx[i][j] = false;
    }
  }
}

void gameResult_TicTacToe() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  
  if (scoreX > scoreO) {
    display.drawLine(54, 11, 74, 31, WHITE); //top left - down right slash
    display.drawLine(74, 11, 54, 31, WHITE); //down left - top right slash
    display.setCursor(44, 48);
    display.println("Won!");
      
  } else if (scoreX < scoreO) {
    display.drawCircle(64, 23, 12, WHITE);
    display.setCursor(44, 48);
    display.println("Won!");
      
  } else if ((scoreX == scoreO) && ((scoreX > 0) || (scoreO > 0))) {
    display.setCursor(44, 28);
    display.println("TIE");
  }

  delay(500);
  selX = 2; //centers the selection box on X axis
  selY = 2; //centers the selection box on Y axis
  scoreX = 0;
  scoreO = 0;
  display.display();
  delay(4000);
}
