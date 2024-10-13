#include <iostream>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;


const float TREE_HORIZONTAL_POSITION = 810;
const float TREE_VERTICAL_POSITION = 0;

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

const int NUM_CLOUDS = 3;
Sprite clouds[NUM_CLOUDS];
float cloudSpeeds[NUM_CLOUDS];
bool cloudsActive[NUM_CLOUDS];

// Where is the player/branch?
enum class side { LEFT, RIGHT, NONE };  // Left or Right
side branchPositions[NUM_BRANCHES];


int main()
{
    cout << "Hello Timber!!!" << endl;
    // Create a window
    VideoMode vm(1920, 1080);
    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

    /* Make a Background Sprite */
    Texture textureBackground;      // Create a texture to hold a graphic on the GPU Texture
    textureBackground.loadFromFile("C:/Users/HashimAbdulAzizHashi/CLionProjects/2D_game0.1/graphics/background.png");  // Load a graphic into the texture textureBackground.
    Sprite spriteBackground;                            // Creat Sprite Background
    spriteBackground.setTexture(textureBackground);     // Attach the texture to the sprite spriteBackground.
    spriteBackground.setPosition(0, 0);             // Set the spriteBackground to cover the screen spriteBackground.

    /* Make a tree Sprite */
    Texture textureTree;
    textureTree.loadFromFile("C:/Users/HashimAbdulAzizHashi/CLionProjects/2D_game0.1/graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(TREE_HORIZONTAL_POSITION, TREE_VERTICAL_POSITION);

    /* Make a Bee Sprite */
    Texture textureBee;
    textureBee.loadFromFile("C:/Users/HashimAbdulAzizHashi/CLionProjects/2D_game0.1/graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);
    // Is the bee currently moving?
    bool beeActive = false;
    // How fast can the bee fly
    float beeSpeed = 0.0f;

    // make 3 cloud sprites from 1 texture
    Texture textureCloud;
    textureCloud.loadFromFile("C:/Users/HashimAbdulAzizHashi/CLionProjects/2D_game0.1/graphics/cloud.png"); // Load 1 new texture
    for (int i = 0; i < NUM_CLOUDS; i++) {
        clouds[i].setTexture(textureCloud);   // 3 New sprites with the same texture
        clouds[i].setPosition(-300, i * 150);   // Position the clouds on the left of the screen at different heights
        cloudsActive[i] = false;    // Are the clouds currently on screen?
        cloudSpeeds[i] = 0.0f;  // How fast is each cloud?
    }

    // Variables to control time itself
    Clock clock;

    // Variable to control the Pause scenarios
    bool paused = true;

    // Initialize some Text Objects
    int score  = 0;
    Text messageText;
    Text scoreText;
    // We need to choose font
    Font font;
    font.loadFromFile("C:/Users/HashimAbdulAzizHashi/CLionProjects/2D_game0.1/fonts/KOMIKAX_.ttf");
    // Set the font for our Texts
    messageText.setFont(font);
    scoreText.setFont(font);
    // Assign the actual message
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");
    // Make it really big
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    // Choose a color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);
    // Position the text
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920 / 2.0f,1080 / 2.0f);
    scoreText.setPosition(20, 20);

    // Initialize Time Bar
    RectangleShape timeBar;
    float timeBarWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920/2) - timeBarWidth/2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarWidth / timeRemaining;

    // Prepare 5 Branches
    Texture textureBranches;
    textureBranches.loadFromFile("C:/Users/HashimAbdulAzizHashi/CLionProjects/2D_game0.1/graphics/branch.png");
    // Set Texture for each Branch
    for(int i = 0; i < NUM_BRANCHES; i++) {
        branches[i].setTexture(textureBranches);
        branches[i].setPosition(-2000, -2000);  // hide it off-screen
        // Set the sprite's origin to dead centre
        // We can then spin it round without changing its position
        branches[i].setOrigin(220, 20);
    }

    // Prepare the player
    Texture texturePlayer;
    texturePlayer.loadFromFile("C:/Users/HashimAbdulAzizHashi/CLionProjects/2D_game0.1/graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);
    // The player starts on the left
    side playerSide = side::LEFT;

    // Prepare the gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("C:/Users/HashimAbdulAzizHashi/CLionProjects/2D_game0.1/graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    // Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("C:/Users/HashimAbdulAzizHashi/CLionProjects/2D_game0.1/graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);
    // Line the axe up with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Prepare the flying log
    Texture textureLog;
    textureLog.loadFromFile("C:/Users/HashimAbdulAzizHashi/CLionProjects/2D_game0.1/graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);
    // Some other useful log related variables
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    // Control the player Input
    bool acceptInput = false;

    // Prepare the sound
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("C:/Users/HashimAbdulAzizHashi/CLionProjects/2D_game0.1/sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);

    // Prepare the sound
    SoundBuffer daethBuffer;
    daethBuffer.loadFromFile("C:/Users/HashimAbdulAzizHashi/CLionProjects/2D_game0.1/sound/death.wav");
    Sound death;
    death.setBuffer(daethBuffer);

    // Prepare the sound
    SoundBuffer outOfTimeBuffer;
    outOfTimeBuffer.loadFromFile("C:/Users/HashimAbdulAzizHashi/CLionProjects/2D_game0.1/sound/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(outOfTimeBuffer);


    while (window.isOpen()) {
        /* Handle the player input */
        Event event;

        while(window.pollEvent(event)) {
            if(event.type == Event::KeyReleased && !paused) {
                // Listen to press key
                acceptInput = true;

                // Hide the axe
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        }

        if(Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();

        if(Keyboard::isKeyPressed(Keyboard::Return)) {
            paused = false;
            // Reset Score and Time Bar
            score = 0;
            timeRemaining = 6.0f;

            // Disable all Branches... Prepare the tree without any branches
            // Note that: if the game started with a branch right above their head, it might be considered unsporting.
            for(int i = 0; i < NUM_BRANCHES; i++)
                branchPositions[i] = side::NONE;

            // Make sure the gravestone is hidden
            spriteRIP.setPosition(675, 2000);

            // Move the player into Start Position
            spritePlayer.setPosition(580, 720);

            // Accept any Input from user
            acceptInput = true;
        }

        // Wrap the player controls to Make sure we are accepting input
        if (acceptInput)
        {
            // More code here next...
            // First handle pressing the right cursor key
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                // Make sure the player is on the right
                playerSide = side::RIGHT;

                score ++;

                // Add to the amount of time remaining
                timeRemaining += (2 / score) + .15;       // Note that: bigger the score gets, the less additional time is added on !!!
                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(1200, 720);

                // Update the branches
                updateBranches(score);

                // Set the log flying to the left
                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;

                chop.play(); // Play Chop sound
            }
            // Handle the left cursor key
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                // Make sure the player is on the left
                playerSide = side::LEFT;

                score++;

                // Add to the amount of time remaining
                timeRemaining += (2 / score) + .15;     // Note that: bigger the score gets, the less additional time is added on !!!
                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(580, 720);

                // Update the branches
                updateBranches(score);

                // Set the log flying to the right
                spriteLog.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;

                chop.play();   // Play chop sound
            }
        }

        if(!paused) {
            /*
             *  Update the scene
             */

            // Measure time
            Time dt = clock.restart();  // The frame time
            // Subtract from the amount of Time Remaining
            timeRemaining -= dt.asSeconds();
            timeBar.setSize(Vector2f(timeBarWidthPerSecond*timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f) {
                // Pause the game
                paused = true;
                // Change the message shown to the player
                messageText.setString("Out of time!!");
                //Reposition the text based on its new size
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
                messageText.setPosition(1920/2.0f, 1080/2.0f);

                // Play the Out of time sound
                outOfTime.play();
            }

            // set up the Bee
            if(!beeActive) {
                // How fast is the Bee
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200; // give Random num (speed) from 200 to 499

                // How height is the Bee
                srand((int)time(0) * 10);
                float Bee_height =  (rand() % 500) + 500; // give Random num (height) from 500 to 999
                spriteBee.setPosition(2000, Bee_height); // 2000 = just-off the screen to right
                beeActive = true;
            }
            else {

                int rond_seed = (rand() % 5);
                srand((int)time(0) *  rond_seed * 10);
                float rond_height = (rand() % 5) - 2;

                // Move the Bee >> (x - delta-distance . y) -ve bcz it moves to left
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()) , spriteBee.getPosition().y - rond_height*0.03);

                // Has the bee reached the left-hand edge of the screen?
                if(spriteBee.getPosition().x < -100)
                    beeActive = false;  // Set it up ready to be a whole new bee next frame
            }

            // Manage the clouds Animation
            for(int i = 0; i < NUM_CLOUDS; i++) {
                if (!cloudsActive[i]) {
                    // How fast is the cloud
                    srand((int)time(0) * ((i+1)*10));
                    cloudSpeeds[i] = (rand() % 200);
                    // How high is the cloud
                    srand((int)time(0) * (i+1)*10);
                    float height = (rand() % ((i+1)*150)) - (150 * (int)((i+1)/2));
                    clouds[i].setPosition(-200, height);  // -200 = just-off left screen
                    cloudsActive[i] = true;
                }
                else {
                    clouds[i].setPosition(clouds[i].getPosition().x + (cloudSpeeds[i] * dt.asSeconds()), clouds[i].getPosition().y);
                    // Has the cloud reached the right hand edge of the screen?
                    if (clouds[i].getPosition().x > 1920)
                        cloudsActive[i] = false;       // Set it up ready to be a whole new cloud next frame
                }
            }

            // Update the score text
            stringstream ss;
            ss << "Score = " << score; scoreText.setString(ss.str());

            // Update the Branch Sprite
            for(int i = 0; i < NUM_BRANCHES; i++) {
                float height = i * 150;
                if(branchPositions[i] == side::LEFT) {
                    branches[i].setPosition(620, height);   // Move the sprite to the left side
                    branches[i].setRotation(180);          // Flip the sprite round the other way
                }
                else if(branchPositions[i] == side::RIGHT) {
                    branches[i].setPosition(1330, height);  // Move the sprite to the right side
                    branches[i].setRotation(0);            // Set the sprite rotation to normal
                }
                else
                    branches[i].setPosition(3000, height);  // Hide the branch
            }

            // Animation of the flying log
            if(logActive) {
                spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
                // Has the log reached the right hand edge of the screen?
                if(spriteLog.getPosition().x > 2000 || spriteLog.getPosition().x < -100) {
                    logActive = false;  // Ready to handle the new log
                    spriteLog.setPosition(810, 720);
                }
            }

            // Handling the death when player squashed
            if(branchPositions[5] == playerSide) {
                // death
                paused = true;
                acceptInput = false;
                spriteRIP.setPosition(spritePlayer.getPosition().x, spritePlayer.getPosition().y);    // Draw the gravitation
                spritePlayer.setPosition(2000, 660);    // Hide the player
                messageText.setString("SQUISHED!!");    // Change the text of the Image
                // Center it on the screen
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
                messageText.setPosition(1920/2.0f, 1080/2.0f);
                // Play death sound
                death.play();
            }
        }

        window.clear();

        /* Draw our game scene here */
        // Draw the Background
        window.draw(spriteBackground);
        // Draw the clouds
        window.draw(clouds[0]);
        window.draw(clouds[1]);
        window.draw(clouds[2]);
        // Draw the branches
        for (int i = 0; i < NUM_BRANCHES; i++)
            window.draw(branches[i]);
        // Draw the tree
        window.draw(spriteTree);
        // Draw the player
        window.draw(spritePlayer);
        // Draw the axe
        window.draw(spriteAxe);
        // Draw the flying log
        window.draw(spriteLog);
        // Draw the gravestone
        window.draw(spriteRIP);
        // Draw the Bee
        window.draw(spriteBee);
        // Draw the score
        window.draw(scoreText);
        // Draw our message text
        if(paused)
            window.draw(messageText);
        // Draw the Time Bar
        window.draw(timeBar);

        window.display();
    }
    return 0;
}


void updateBranches(int seed) {
    // Move all the branches down one place
    // Note that: if each branch took tha position(left or right) of the before it that will look they move down !!!
    for(int j = NUM_BRANCHES - 1; j > 0; j--) {
        branchPositions[j] = branchPositions[j - 1];
    }

    // Spawn a new branch at position 0
    // Now we need to change the position(left/right) oh the first branch randomly
    // LEFT, RIGHT or NONE
    srand((int)time(0)+seed);
    int r = (rand() % 5);

    switch (r) {
        case 0:
            branchPositions[0] = side::LEFT;
            break;
        case 1:
            branchPositions[0] = side::RIGHT;
            break;
        default:
            branchPositions[0] = side::NONE;
            break;
    }
}



