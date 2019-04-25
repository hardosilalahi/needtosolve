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
  int O_ready;
  
  int X_row; int X_column;  int X_turn;
  int O_row; int O_column;  int O_turn;

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
  if((scoreX == 10) || (scoreO == 10))  {continueGame = false;}
  
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

//  if (!client.connected()) {reconnect();}
//  if(!client.loop())
//    client.connect("ESP8266_2");

//  static char O_ready[7];
//  dtostrf(loading_tictactoe, 1, 1, O_ready);
//  client.publish("tictactoe/O_ready/out", O_ready);
    String O_readyStr;
    int O_ready = 1;
    
    O_readyStr = String(O_ready);
    
    http.begin("http://192.168.0.36:1880/tictactoe/player_o/post/signal");
    http.addHeader("Content-Type", "text/plain");

    int O_sendReady = http.POST(O_readyStr);
    String O_payloadready = http.getString();
    Serial.println("string payload ready: " + O_payloadready);
    http.end();   

///////////////////TICTACTOE GET HTTP REQUEST//////////////////////
  http.begin("http://192.168.0.36:1880/tictactoe/player_x/get/signal");
  
  int pX_signalGet = http.GET();
  String pX_payloadSignal = http.getString();
  X_ready = pX_payloadSignal.toInt();
  http.end();
//////////////
  http.begin("http://192.168.0.36:1880/tictactoe/player_x/get/turn");
  
  int pX_turnGet = http.GET();
  String pX_payloadTurn = http.getString();
  X_turn = pX_payloadTurn.toInt();
  http.end();
//////////////
  http.begin("http://192.168.0.36:1880/tictactoe/player_x/get/row");
  
  int pX_rowGet = http.GET();
  String pX_payloadRow = http.getString();
  X_row = pX_payloadRow.toInt();
  http.end();
//////////////
  http.begin("http://192.168.0.36:1880/tictactoe/player_x/get/column");
  
  int pX_columnGet = http.GET();
  String pX_payloadColumn = http.getString();
  X_column = pX_payloadColumn.toInt();
  http.end();
////////////////////////////////////////////////////////////////////

  display.clearDisplay();
  if((scoreX == 10) || (scoreO == 10))  {continueGame = false;}

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
    }
  } else {
    Serial.println("WAITING FOR PLAYER 1!");
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(20, 16);
    display.println("WAIT FOR   PLAYER 1");
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

  Serial.print("left  : ");   Serial.println(left);
  Serial.print("right : ");   Serial.println(right);
  Serial.print("up    : ");   Serial.println(up);
  Serial.print("down  : ");   Serial.println(down);  
  Serial.print("enter : ");   Serial.println(enter);

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
  Serial.print("X_turn: "); Serial.println(X_turn);
  Serial.print("O_turn: "); Serial.println(O_turn);
  Serial.println("");

  Serial.print("Player: "); Serial.println(player);
  Serial.println("");
  
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
            
            String O_rowStr;
            
            O_rowStr = String(O_row);
            
            http.begin("http://192.168.0.36:1880/tictactoe/player_o/post/row");
            http.addHeader("Content-Type", "text/plain");
            
            int O_sendRow = http.POST(O_rowStr);
            String O_payloadRow = http.getString();
            Serial.println("string payload row: " + O_payloadRow);
            http.end();     
////////////////////////////
            String O_columnStr;
            
            O_columnStr = String(O_column);
                        
            http.begin("http://192.168.0.36:1880/tictactoe/player_o/post/column");
            http.addHeader("Content-Type", "text/plain");
            
            int O_sendColumn = http.POST(O_columnStr);
            String O_payloadColumn = http.getString();
            Serial.println("string payload column: " + O_payloadColumn);
            http.end();    
//////////////////////////////
            String O_turnStr;

            O_turnStr = String(O_turn);

            http.begin("http://192.168.0.36:1880/tictactoe/player_o/post/turn");
            http.addHeader("Content-Type", "text/plain");
            
            int O_sendTurn = http.POST(O_turnStr);
            String O_payloadTurn = http.getString();
            Serial.println("string payload: " + O_payloadTurn);
            http.end();      
//////////////////////////
            delay(10);
//////////////////////////////
            O_turn = 0;

            O_turnStr = String(O_turn);

            http.begin("http://192.168.0.36:1880/tictactoe/player_o/post/turn");
            http.addHeader("Content-Type", "text/plain");
            
            O_sendTurn = http.POST(O_turnStr);
            O_payloadTurn = http.getString();
            Serial.println("string payload: " + O_payloadTurn);
            http.end();              
/////////////////////////////
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
  else if (player == 'T') {Serial.println("DRAW. NOBODY WINS");}

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

  delay(100);
  selX = 2; //centers the selection box on X axis
  selY = 2; //centers the selection box on Y axis
  scoreX = 0;
  scoreO = 0;
  display.display();
  delay(500);
}
