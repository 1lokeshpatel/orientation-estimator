import processing.serial.*;
Serial port;    // Create an object from Serial class
int xPos = 1;         // horizontal position of the graph
float inByte = 0;
float roll, accelRoll, gyroRoll;
// table to store sensor data into
Table sensorDataTable;

void setup() {
  port = new Serial(this, "COM8", 19200);
  // don't generate a serialEvent() unless you get a newline character:
  port.bufferUntil('\n');
  
  // init table
  sensorDataTable = new Table();
  // nice to have: table column names
  sensorDataTable.addColumn("Roll");
  sensorDataTable.addColumn("Accelerometer Roll");
  sensorDataTable.addColumn("Gyro Roll");
  roll = 0;
  accelRoll = 0;
  gyroRoll = 0;
}

void draw() {
background(255);
  
  String sensorText = String.format("Temperature: %.2f Humidity: %.2f  Moisture: %.2f", roll, accelRoll, gyroRoll);
  float textWidth = textWidth(sensorText);
  float textX = (width - textWidth) / 2;
  fill(255);  
  rect(textX - 10, 14, textWidth + 20, 21);
  fill(0);
  text(sensorText, textX, 30);
}

void serialEvent (Serial myPort) {
  // get the ASCII string:
  String inString = myPort.readStringUntil('\n');

  if (inString != null) {

      // trim off any whitespace:
      inString = trim(inString);
      // convert to an int and map to the screen height:
      //inByte = float(inString);
      println(inByte);
      //roll = inByte;
      //inByte = map(inByte, 0, 1023, 0, height);
      float[] sensorData = float(split(inString, ','));
      if(sensorData.length == 3){
        roll = sensorData[0];
        accelRoll = sensorData[1];
        gyroRoll = sensorData[2];
      }
      // add data to table (using column names)
      TableRow newRow = sensorDataTable.addRow();
      newRow.setFloat("Roll", roll);
      newRow.setFloat("Accelerometer Roll", accelRoll);
      newRow.setFloat("Gyro Roll", gyroRoll);
  }
}

// save on 's'
void keyPressed(){
  if(key == 's'){
    // save table to disk as .TSV instead of .CSV
    // potentially avoiding .CSV and float values conflicts (depending on sys.language) 
    saveTable(sensorDataTable, "data/sensorDataPitch.csv");
  }
}

// save on exit
void exit(){
  saveTable(sensorDataTable, "data/sensorDataPitch.csv");
  super.exit();
}
