#include <mygba.h>
#include <string.h>

// Forward Declarations
void QueryMainButtons(); // Query for input
void MainScreen();  // Draw main screen
void HelpScreen();  // Draw main screen
void FillCustomFoods(); // fill custom foods structure
void AddAFood(int calories, char* desc, int recipeNum); // add a single food
void DisplayCurrentRecipe(); // Displays the current recipe
void ClearLine(int line); // clear a line of text

typedef struct
{
    char desc[20];
    int calories;
} foodItem;

typedef struct
{
    foodItem foods[10];
    int currentRecipe;
    int numRecipies;
} recipeBook;


// Global Kill Switch
int killvar;

// Local Var for Calorie Goal
int calorieGoal;
int currentCalories;

// Global recipe book
recipeBook recipies;

int main(void)
{
    
    // Basic Defaults
    calorieGoal = 2000;
    currentCalories = 0;
    
    // Fill up the recipe book
    FillCustomFoods();
    
    //If the RAM is already organized in HAM format...
    if(ham_InitRAM(RAM_TYPE_SRAM_256K))
    {
        ham_LoadIntFromRAM("calorie goal",&calorieGoal);
        ham_LoadIntFromRAM("current calories",&currentCalories);

    }
    else
    {
        //RAM was not in HAM format, so it was initialized now.
        ham_SaveIntToRAM("calorie goal",2000);
        ham_SaveIntToRAM("current calories",0);
    }
    
    // main ham init functions
    ham_Init();
    ham_InitPad();
    
    // Draw the Initial Screen
    MainScreen();

    // killvar loop var
    killvar = 0;
    while(killvar == 0)
    {
        // query for input
        QueryMainButtons();
    }
    
    // Save out Values before Exit Screen
    ham_SaveIntToRAM("calorie goal",calorieGoal);
    ham_SaveIntToRAM("current calories",currentCalories);

    // end of program
    return 0;
}

// Query Buttons for Input
void QueryMainButtons()
{
    int exit_key_pressed = 0;
    // Loop until a direction on the D-Pad is pressed
    while(exit_key_pressed == 0)
    {
        // Read pad states
        ham_UpdatePad();

        // Access the Pad variable
        // Check if button down has been pressed
        if(Pad.Pressed.Down)
        {
            if(currentCalories >= 10)
            {
                currentCalories -= 10;
                ClearLine(7);
                ClearLine(10);
                ham_DrawText(0,7,"Current Calories: %d",currentCalories);
                ham_DrawText(0,10,"Calories Remaining: %d",(calorieGoal - currentCalories));
            }
        }
        else if(Pad.Pressed.Up)
        {
                currentCalories += 10;
                ClearLine(7);
                ClearLine(10);
                ham_DrawText(0,7,"Current Calories: %d",currentCalories);
                ham_DrawText(0,10,"Calories Remaining: %d",(calorieGoal - currentCalories));
        }
        else if(Pad.Pressed.Left)
        {
            if(recipies.currentRecipe == 0)
            {
                recipies.currentRecipe = recipies.numRecipies - 1;
            }
            else
            {
                recipies.currentRecipe -= 1;
            }
            DisplayCurrentRecipe();
        }
        else if(Pad.Pressed.Right)
        {
            if(recipies.currentRecipe == (recipies.numRecipies - 1))
            {
                recipies.currentRecipe = 0;
            }
            else
            {
                recipies.currentRecipe += 1;
            }
            DisplayCurrentRecipe();
        }
        else if(Pad.Pressed.A)
        {
                currentCalories += recipies.foods[recipies.currentRecipe].calories;
                ClearLine(7);
                ClearLine(10);
                ham_DrawText(0,7,"Current Calories: %d",currentCalories);
                ham_DrawText(0,10,"Calories Remaining: %d",(calorieGoal - currentCalories));
        }
        else if(Pad.Pressed.L && Pad.Pressed.R)
        {
            // Change key_pressed to true
            exit_key_pressed = 1;

            // Close the main program
            killvar = 1;
        }
        else if(Pad.Pressed.Start)
        {
            // Change key_pressed to true
            exit_key_pressed = 1;
            
            //De-ref text
            ham_DeInitText();
            // draw the help screen
            HelpScreen();
        }

    } // end While exit_key_pressed
    
}

void MainScreen()
{
    // Initialize the text display system on BGMODE 0
    ham_InitText(0);
    
    // Draw title to the screen
    ham_DrawText(7,0,"Calorie Counter");

    // Draw instructions to the screen
    ham_DrawText(2,18,"Start for Help and Options");
    ham_DrawText(0,19,"Press L and R to Save and Exit");
    
    // Draw Current Calories and Calorie Goal
    ham_DrawText(0,7,"Current Calories: %d",currentCalories);
    ham_DrawText(0,8,"Calorie Goal: %d",calorieGoal);
    ham_DrawText(0,10,"Calories Remaining: %d",(calorieGoal - currentCalories));
    ham_DrawText(0,11,"--------------------");
    DisplayCurrentRecipe();
    
}
// Displays the current recipe
void DisplayCurrentRecipe()
{
    ham_DrawText(0,12,"                              ");
    ham_DrawText(0,13,"                              ");
    ham_DrawText(0,12,"Press A to Add:");
    ham_DrawText(0,13," %s (%d calories)",
        recipies.foods[recipies.currentRecipe].desc,
        recipies.foods[recipies.currentRecipe].calories);

}

// clear a line of text
void ClearLine(int line)
{
    ham_DrawText(0,line,"                            ");
}

void HelpScreen()
{
    // Initialize the text display system on BGMODE 0
    ham_InitText(0);

    // Draw title to the screen
    ham_DrawText(6,0,"Help And Options");

    // Draw instructions to the screen
    ham_DrawText(0,19,"Press L and R to Save and Exit");
    ham_DrawText(3,18,"Start to Save and Return");
    ham_DrawText(0,15,"Pad Up/Down to Change Goal");
    ham_DrawText(0,14,"Pad Left/Right to Change Cals");
    
    // Draw Current Calories and Calorie Goal
    ham_DrawText(0,7,"Current Calories: %d",currentCalories);
    ham_DrawText(0,8,"Calorie Goal: %d",calorieGoal);

    int exit_key_pressed = 0;
    // Loop until a direction on the D-Pad is pressed
    while(exit_key_pressed == 0)
    {
        // Read pad states
        ham_UpdatePad();

        // Access the Pad variable
        // Check if button down has been pressed
        if(Pad.Pressed.Left)
        {
            if(currentCalories >= 10)
            {
                currentCalories -= 10;
                ham_DrawText(0,7,"Current Calories: %d",currentCalories);
            }
        }
        else if(Pad.Pressed.Right)
        {
                currentCalories += 10;
                ham_DrawText(0,7,"Current Calories: %d",currentCalories);
        }
        if(Pad.Pressed.Up)
        {
            calorieGoal += 100;
            ham_DrawText(0,8,"Calorie Goal: %d",calorieGoal);
        }
        if(Pad.Pressed.Down)
        {
            if(calorieGoal >= 100)
            {
                calorieGoal -= 100;
                ham_DrawText(0,8,"Calorie Goal: %d",calorieGoal);
            }
        }
        else if(Pad.Pressed.L && Pad.Pressed.R)
        {
            // Change key_pressed to true
            exit_key_pressed = 1;

            // Close the main program
            killvar = 1;
        }
        else if(Pad.Pressed.Start)
        {
            //Change key_pressed to true
            exit_key_pressed = 1;
            
            //De-ref text and switch to main
            ham_DeInitText();
            MainScreen();
        }
        
    } // end While exit_key_pressed

}

// Fill up our recipe book
void FillCustomFoods()
{
    // What is the current recipe and total number?
    recipies.currentRecipe = 0;
    recipies.numRecipies = 10;
    
    // Berry Water
    AddAFood(25,"Berry Water",0);
    
    // Quaker Instant Oatmeal
    AddAFood(155,"Quaker Oats",1);
    
    // Corn Dog
    AddAFood(165, "Vegan Corn Dog",2);
    
    // rice cake
    AddAFood(40,"Rice Cake",3);
    
    // snackbar
    AddAFood(100,"Snac kBar",4);
    
    // Granola
    AddAFood(100,"Granola",5);
    
    // Shrimp Lo Mein
    AddAFood(370,"Shrimp Bowl",6);
    
    // Cheese Crackers
    AddAFood(80,"Cheese Crackers",7);
    
    // Fish Taco
    AddAFood(290,"Del Fish Taco",8);
    
    // French Fries
    AddAFood(160,"French Frie s",9);
} 

// Add a single food to recipe book
void AddAFood(int calories, char* desc, int recipeNum)
{
    strcpy(recipies.foods[recipeNum].desc,desc);
    recipies.foods[recipeNum].calories = calories;
}


/* END OF FILE */
