#define BATTERY_CELL_NORMAL 0
#define BATTERY_CELL_LOW 1
#define BATTERY_CELL_HIGH 2
#define TEMPERATURE_NORMAL 0
#define TEMPERATURE_LOW 1
#define TEMPERATURE_HIGH 2

//Checking state of batteries

/*


1 NORMAL
2 HIGH
0 LOW
0 CHECK



CHECK && BATT_NORMAL -> 0
if output = 0 -> Battery is fine 
if !output = 0 -> Battery is not fine
    {

        OUTPUT == 0:  
                            if equal -> batt low
                            if !equal -> batt high

    }
*/

//evaluating batt state

bool check_batt_state(int batt_state){

if (batt_state == BATTERY_CELL_NORMAL)
{
    return true;
}
 
return false;

}

bool err_batt_low(int batt_state){
if(batt_state == BATTERY_CELL_LOW) return true; //returns true if the battery is low
return false; //battery is high

}


bool check_temp_state(int temp_state){

if (temp_state == TEMPERATURE_NORMAL)
{
    return true;
}
 
return false;

}

bool err_temp_low(int temp_state){
if(temp_state == TEMPERATURE_LOW) return true; //returns true if the temperature is low
return false; //temperature is high

}
