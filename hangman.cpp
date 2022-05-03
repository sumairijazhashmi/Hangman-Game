#include<iostream>
#include<string>
#include<cstdlib>

#include <termios.h>
#include <unistd.h>

using namespace std;

//input word
//show blanks
//randomly show 2 letters
//input letter
//if letter in word
  //print word with new letter
// else
  //wrong output
  //print original with blanks
//if wrong tries--
//total tries = 10


class hangMan {
  private:
    char input_letter;
    string word;
    string guess;
    int tries;
  public:
    hangMan(string input_word); //initialize with 10 tries and input word
    void blanks(); //make a word with all blanks
    void initial_letters(); //randomly show 2 letters from the blank word
    string show_current(); //show the current progress of the player - show the current word with blanks
    void input(char letter); //input a letter from the user
    bool check(); //check if the letter input is present inside the word
    void update_guess(); //update the word with blanks
    int get_tries(); //get the number of tries left
    void wrong_guess(); //decrease the number of tries left if guessed wrong
    string get_word(); //return the actual word
};

//constructor
hangMan::hangMan(string input_word) {
    tries = 10;
    word = input_word;
    guess = "";
    input_letter = '*';
}


void hangMan::wrong_guess() {
  tries--;
}

int hangMan::get_tries() {
  return tries;
}

void hangMan::blanks() {
  guess = word;
  for (int i=0; i < word.length(); i++) {
    guess[i] = '_';
  }
}

void hangMan::initial_letters() {
  int letter_index = rand() % word.length();
  guess[letter_index] = word[letter_index];
  letter_index = rand() % word.length();
  guess[letter_index] = word[letter_index];
}

string hangMan::show_current() {
  return guess;
}

void hangMan::input(char letter) {
  input_letter = letter;
}

bool hangMan::check() {
  bool found = false;
  for (int i = 0; i < word.length(); i++) {
    if (input_letter == word[i]) {
      found = true;
    }
  }
  return found;
}

void hangMan::update_guess() {
  for (int i = 0; i < word.length(); i++) {
    if (input_letter == word[i]) {
      guess[i] = input_letter;
    }
  }
}

string hangMan::get_word() {
  return word;
}

//main game loop
void game_loop() {
  termios oldt;
  tcgetattr(STDIN_FILENO, &oldt);
  termios newt = oldt;
  newt.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  string input_word;
  cout << "hello, welcome to hangman! Player 1, please enter a word to continue:\n";
  cin >> input_word;

  hangMan game(input_word);
  game.blanks();

    //SetConsoleMode(hStdin, mode);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  cout << "great! Player 2, this is the word that you have to guess:\n";
  game.initial_letters();
  cout << game.show_current() << endl;
  cout << "Player 2, you can only choose a wrong letter 10 times, after that you will lose. Good luck!\n";

  char input;

  while (game.get_tries() > 0) {
    if(game.show_current() == game.get_word()) {
      cout << "Congrats Player 2, you have won!\n";
      return;
    }
    cout << "Enter a letter:\n";
    cin >> input;
    game.input(input);
    if (game.check()) {
      cout << "correct!\n";
      game.update_guess();
      cout << game.show_current() << endl;
    }
    else {
      cout << "wrong!\n";
      game.wrong_guess();
      cout << "you have " << game.get_tries() << " left.\n";
      cout << game.show_current() << endl;
      if(game.get_tries() == 0) {
        cout <<"Player 2, you have lost! :(\n";
        cout <<"The word was: " << game.get_word() << endl;
        cout << "Congrats Player 1, you have won!\n";
      }
    }
  }
}

int main() {

  srand(0);
  game_loop();
  return 0;
}
