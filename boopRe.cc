#include <iostream>
#include <iomanip>
#include "boopRe.h"
#include "colors.h"
using namespace std;
using namespace main_savitch_14;

/**
 * @brief Construct a new Boop:: Boop object
 * 
 */
Boop::Boop(){
    move_type = 0;
    player1_small = 8;
    player1_large = 0;
    player2_small = 8;
    player2_large = 0;
    current_player = 1; //will switch between 1 and 2 for every turn
}
/**
 * @brief restarts the game to the beginning state
 * 
 */
void Boop::restart(){
    // move_type = 0;
    player1_small = 8;
    player1_large = 0;
    player2_small = 8;
    player2_large = 0;
    for(int i = 0; i < 6; i++){
        for (int j = 0; j < 6; j++){
            board[i][j].change_owner(0 , '\0');
        }
    }
    game::restart();
    // board[1][1].change_owner(2 , 'P');
    // board[2][2].change_owner(2 , 'P');
    // board[3][3].change_owner(2 , 'C');

}
/**
 * @brief Outputs the Board, any pieces on the board, and the number of pieces left for each player
 * 
 */
void Boop::display_status()const{

    int row_part = 0; //checks if we are at the top or the bottom of a single row
    int row = -1; //the current row we are at
    for(int i = 0; i < board_size*3; i++){
        if(i % 3 == 0){ //makes the horizontal lines every three newlines
            cout << setw(105) << setfill('-') << endl;
            row_part = 0;
            row++;
        }
        cout <<"\n";
        for(int j = 0; j < board_size; j++){
            if(j == 5)
            { //last column if the plane is selected 
                if(row_part == 0){
                    cout << "|" << board[row][j].getColor() << board[row][j].charTop() << RESET << "|"; //top part
                }else if(row_part == 1){
                    cout << "|" << board[row][j].getColor() << board[row][j].charMiddle() << RESET << "|" << " Row " << row; //middle part
                }else{
                    cout << "|" << board[row][j].getColor() << board[row][j].charBottom() << RESET << "|"; //bottom part
                }
                
            }
            else
            { //every other column if plane is selected
                if(row_part == 0){
                    cout << "|" << board[row][j].getColor() << board[row][j].charTop() << RESET << ""; //top part
                }else if(row_part == 1){
                    cout << "|" << board[row][j].getColor() << board[row][j].charMiddle() << RESET << ""; //middle part
                }else{
                    cout << "|" << board[row][j].getColor() << board[row][j].charBottom() << RESET << ""; //bottom part
                }
            }
        }
        
        row_part++;
    }
    cout << endl;
    //the labels for the columns
    for(int i = 0; i < 6; i++){
        cout << setfill(' ') << "    Column: " << i << "   ";
    }
    cout << endl;
    if(!is_game_over()){ //prevents these from being displayed when the game is over
        cout << RED << "Player 1 Large: " << player1_large << " Player 1 Small: "<< player1_small << RESET << endl;
        cout << BLUE << "Player 2 Large: " << player2_large << " Player 2 Small: "<< player2_small << RESET << endl;
        cout << CYAN << "Planes (P) are large and Helicopters (C) are small" << RESET << endl;
        cout << "\nPlease input in this order: Row , Column, Piece (P or C)\n";
        cout << "Evaluated Number: " << evaluate() << endl;
    }
}
/**
 * @brief prints the board without the number of pieces and prompt for another move
 * 
 */
void Boop::board_only()const{
    int row_part = 0; //checks if we are at the top or the bottom of a single row
    int row = -1; //the current row we are at
    for(int i = 0; i < board_size*3; i++){
        if(i % 3 == 0){ //makes the horizontal lines every three newlines
            cout << setw(105) << setfill('-') << endl;
            row_part = 0;
            row++;
        }
        cout <<"\n";
        for(int j = 0; j < board_size; j++){
            if(j == 5)
            { //last column if the plane is selected 
                if(row_part == 0){
                    cout << "|" << board[row][j].getColor() << board[row][j].charTop() << RESET << "|"; //top part
                }else if(row_part == 1){
                    cout << "|" << board[row][j].getColor() << board[row][j].charMiddle() << RESET << "|" << " Row " << row; //middle part
                }else{
                    cout << "|" << board[row][j].getColor() << board[row][j].charBottom() << RESET << "|"; //bottom part
                }
                
            }
            else
            { //every other column if plane is selected
                if(row_part == 0){
                    cout << "|" << board[row][j].getColor() << board[row][j].charTop() << RESET << ""; //top part
                }else if(row_part == 1){
                    cout << "|" << board[row][j].getColor() << board[row][j].charMiddle() << RESET << ""; //middle part
                }else{
                    cout << "|" << board[row][j].getColor() << board[row][j].charBottom() << RESET << ""; //bottom part
                }
            }
        }
        
        row_part++;
    }
    cout << endl;
    //the labels for the columns
    for(int i = 0; i < 6; i++){
        cout << setfill(' ') << "    Column: " << i << "   ";
    }
    cout << endl;
}
/**
 * @brief moves the pieces around the move made by the user, makes sure if they go over the board they get sent back to the player, and makes sure 
 *          that the only piece that moves is that smaller or equal to the move made
 * 
 * @param input 
 */
void Boop::pieceAround(const string input){
    //outputs the available spots that need to be checked for potential spaces on the spot
    //does not acount for the difference in large and small pieces
    int row = int(input[0] - '0');
    int column = int(input[1] - '0');
    int originalColumn = column;
    int originalRow = row;


    /**
     * @brief The left column detection, moving of the detected pieces, and then removing any pieces pushed out of bounds
     * 
     */
    column--; //go to the column to the left
    if(column != -1){ //goes too far to the left and out of bounds
        // cout << "Left Column: " << endl;
        for(int i = row + 1; i >= row - 1; i--){
            if(i > -1 && i < 6){ 
            //if the space in the column to the left is either above or below the playable space then it will not go to that space
                if(board[i][column].isSpaceClosed()){ //if the space has a piece on it
                    if(i - (originalRow - i) > -1 && i - (originalRow - i) < 6 && column - 1 > -1){
                        //if the piece that would be moved is larger than the placed piece then dont move the said piece
                        if(board[int(input[0] - '0')][int(input[1] - '0')].getPiece() == 'C' && board[i][column].getPiece() == 'P'){
                            break;
                        }
                        //if the space the spot would be pushed too is full then dont move that piece
                        if(board[i - (originalRow - i)][column - (originalColumn - column)].getPiece() != '\0'){
                            break;
                        }
                        //moves the piece in accordance to the rules
                        //takes the difference between the two pieces
                        //moves the piece left in the magnitude of the difference in spaces
                        board[i - (originalRow - i)][column - (originalColumn - column)].change_owner(
                            board[i][column].getOwner(),
                            board[i][column].getPiece()
                        );

                        // resets the space that is now empty that used to hold the piece that was moved
                        board[i][column].change_owner(0 , '\0');
                    }else{
                        if(board[int(input[0] - '0')][int(input[1] - '0')].getPiece() == 'C' && board[i][column].getPiece() == 'P'){
                            break;
                        }
                        if(board[i][column].getPiece() == 'P'){ //if the removed piece is P
                            if(board[i][column].getOwner() == 1){
                                player1_large++; //return the large piece to player 1
                            }else{
                                player2_large++; //return the large piece to player 2
                            }
                        }else{ //removed piece is C
                            if(board[i][column].getOwner() == 1){
                                player1_small++; //retuns the small piece to player 1
                            }else{
                                player2_small++; //returns the small piece to player 2
                            }
                        }
                        // resets the space that is now empty that used to hold the piece that was moved
                        board[i][column].change_owner(0 , '\0');
                    }
                }
            }
        }
    }
    column++; //reset the column to the correct spot

    /**
     * @brief The right column detection, moving of the detected pieces, and then removing any pieces pushed out of bounds
     * 
     */
    column++; //go to the column to the right
    if(column != 6){ //goes to far to the left and out of bounds
        // cout << "Right Column: " << endl;
        for(int i = row+ 1; i >= row - 1; i--){
            if(i > -1 && i < 6){ 
            //if the space in the column to the left is either above or below the playable space then it will not go to that space
                if(board[i][column].isSpaceClosed()){ //if the space has a piece on it
                    if(i + (i - originalRow) > -1 && i + (i - originalRow) < 6 && column + 1< 6){
                        //if the piece that would be moved is larger than the placed piece then dont move the said piece
                        if(board[int(input[0] - '0')][int(input[1] - '0')].getPiece() == 'C' && board[i][column].getPiece() == 'P'){
                            break;
                        }
                        //if the space the spot would be pushed to is full then dont move that piece
                        if(board[i + (i - originalRow)][column - (originalColumn - column)].getPiece() != '\0'){
                            break;
                        }
                        //moves the piece in accordance to the rules
                        //takes the difference between the two pieces
                        //moves the piece left in the magnitude of the difference in spaces
                        board[i + (i - originalRow)][column - (originalColumn - column)].change_owner(
                            board[i][column].getOwner(),
                            board[i][column].getPiece()
                        );

                        // resets the space that is now empty that used to hold the piece that was moved
                        board[i][column].change_owner(0 , '\0');
                    }else{
                        if(board[int(input[0] - '0')][int(input[1] - '0')].getPiece() == 'C' && board[i][column].getPiece() == 'P'){
                            break;
                        }
                        if(board[i][column].getPiece() == 'P'){ //if the removed piece is P
                            if(board[i][column].getOwner() == 1){
                                player1_large++; //return the large piece to player 1
                            }else{
                                player2_large++; //return the large piece to player 2
                            }
                        }else{ //removed piece is C
                            if(board[i][column].getOwner() == 1){
                                player1_small++; //retuns the small piece to player 1
                            }else{
                                player2_small++; //returns the small piece to player 2
                            }
                        }
                        // resets the space that is now empty that used to hold the piece that was moved
                        board[i][column].change_owner(0 , '\0');
                    }
                    // cout << "New Row " << i + (i - originalRow) << " New Column Diff " << column - (originalColumn - column) << endl; 
                }
            }
        }
    }
    column--; //reset the column to the correct spot

    /**
     * @brief The below row detection, moving of the detected piece, and then removing any piece pushed out of bounds
     * 
     */
    row++; //go to the spot directly below the move
    if(row != 6){
        if(board[row][column].isSpaceClosed()){ //if the space has a piece on it
            if(board[row - 1][column].getPiece() == 'C' && board[row][column].getPiece() == 'P'){
                //dummy if statement that ensures that if the places piece 
                //is a chopper and the piece it wants to move is a Plane then nothing will happened
            }else if(board[row + 1][column].getPiece() != '\0' && row + 1 < 6){
                //another dummy
                //if the destination piece is closed then dont do any of the moving
            }
            else if(row + 1 < 6){ //if the moved piece will stay on the board
                board[row + 1][column].change_owner(
                    board[row][column].getOwner(),
                    board[row][column].getPiece()
                ); 
                board[row][column].change_owner(0 , '\0');
            }else{
                if(board[row - 1][column].getPiece() == 'C' && board[row][column].getPiece() == 'P'){
                //dummy if statement that ensures that if the places piece 
                //is a chopper and the piece it wants to move is a Plane then nothing will happened
                }
                if(board[row][column].getPiece() == 'P'){ //if the removed piece is P
                    if(board[row][column].getOwner() == 1){
                        player1_large++; //return the large piece to player 1
                    }else{
                        player2_large++; //return the large piece to player 2
                    }
                }else{ //removed piece is C
                    if(board[row][column].getOwner() == 1){
                        player1_small++; //retuns the small piece to player 1
                    }else{
                        player2_small++; //returns the small piece to player 2
                    }
                }
                // resets the space that is now empty that used to hold the piece that was moved
                board[row][column].change_owner(0 , '\0');
            }
        }
    }
    row--; //back to correct spot

    /**
     * @brief The above row detection, moving of the detected piece, and then removing any piece pushed out of bounds
     * 
     */
    row--; //go to the spot above the given move
    if(row != -1){
        if(board[row][column].isSpaceClosed()){ //if the space has a piece on it
            if(board[row + 1][column].getPiece() == 'C' && board[row][column].getPiece() == 'P'){
                //dummy if statement that ensures that if the places piece 
                //is a chopper and the piece it wants to move is a Plane then nothing will happened
            }else if(board[row - 1][column].getPiece() != '\0' && row - 1 > -1){
                //another dummy
                //if the destination piece is closed then dont do any of the moving
            }
            else if(row - 1 > -1){ //if the moved piece will stay on the board
                board[row - 1][column].change_owner(
                    board[row][column].getOwner(),
                    board[row][column].getPiece()
                ); 
                board[row][column].change_owner(0 , '\0');
            }else{
                if(board[row - 1][column].getPiece() == 'C' && board[row][column].getPiece() == 'P'){
                //dummy if statement that ensures that if the places piece 
                //is a chopper and the piece it wants to move is a Plane then nothing will happened
                }
                if(board[row][column].getPiece() == 'P'){ //if the removed piece is P
                    if(board[row][column].getOwner() == 1){
                        player1_large++; //return the large piece to player 1
                    }else{
                        player2_large++; //return the large piece to player 2
                    }
                }else{ //removed piece is C
                    if(board[row][column].getOwner() == 1){
                        player1_small++; //retuns the small piece to player 1
                    }else{
                        player2_small++; //returns the small piece to player 2
                    }
                }
                // resets the space that is now empty that used to hold the piece that was moved
                board[row][column].change_owner(0 , '\0');
            }
        }
    }
    row++; //back to correct spot
}

/*
    Board Checks







*/
/**
 * @brief if there are no pieces available for the current player
 * 
 * @return true 
 * @return false 
 */
bool Boop::no_pieces(){
    if(player1_large == 0 && player1_small == 0 && current_player == 1){
        return true;
    }
    if(player2_large == 0 && player2_small == 0 && current_player == 2){
        return true;
    }
    return false;
}
/**
 * @brief returns true if there is 3 in a row of any kind
 * 
 */
bool Boop::three_in_row()const{
    int rowDiffOne = 0;
    int rowDiffTwo = 0;
    int columnDiffOne = 0;
    int columnDiffTwo = 0;
    int repsOne = 0;
    int repsTwo = 0;
    int numOfDirections = 4;
    for(int i = 0; i < numOfDirections; i++){
        if(i == 0){//checking horizontal matches first
            repsOne = 6; //outer for loop
            repsTwo = 4; //inner for loop
            columnDiffOne = 1;
            columnDiffTwo = 2;
            rowDiffOne = 0;
            rowDiffTwo = 0;
        }else if(i == 1){//checking for vertical matches
            repsOne = 4; //outer for loop
            repsTwo = 6; //inner for loop
            rowDiffOne = 1;
            rowDiffTwo = 2;
            columnDiffOne = 0;
            columnDiffTwo = 0;
        }else if(i == 2){ //top left to bottom right diagonal
            repsOne = 4; //outer for loop
            repsTwo = 4; //inner for loop
            rowDiffOne = 1;
            rowDiffTwo = 2;
            columnDiffOne = 1;
            columnDiffTwo = 2;
        }else if(i == 3){ //bottom left to top right diagonal
            repsOne = 4; //outer for loop
            repsTwo = 4; //inner for loop
            rowDiffOne = -1;
            rowDiffTwo = -2;
            columnDiffOne = 1;
            columnDiffTwo = 2;
        }
        for(int i = 0; i < repsOne; i++){
            for(int j = 0; j < repsTwo; j++){
                if(
                    //board[i][j].getPiece() == board[i + rowDiffOne][j + columnDiffOne].getPiece() && //1 and 2 are equal piecewise
                    //board[i][j].getPiece() == board[i + rowDiffTwo][j + columnDiffTwo].getPiece() && //1 and 3 are equal piecewise
                    //all the spaces have an actual piece on them
                    board[i][j].isSpaceClosed() && board[i + rowDiffOne][j + columnDiffOne].isSpaceClosed() && board[i + rowDiffTwo][j + columnDiffTwo].isSpaceClosed() && 
                    board[i][j].getOwner() == board[i + rowDiffOne][j + columnDiffOne].getOwner() &&  //1 and 2 are equal owner wise
                    board[i][j].getOwner() == board[i + rowDiffTwo][j + columnDiffTwo].getOwner()//1 and 3 are equal owner wise
                ){
                    if(
                        //if there is 3 Planes in a Row, do not ask for the removal of pieces because someone won
                        board[i][j].getPiece() == board[i + rowDiffOne][j + columnDiffOne].getPiece() && //1 and 2 are equal piecewise
                        board[i][j].getPiece() == board[i + rowDiffTwo][j + columnDiffTwo].getPiece() && //1 and 3 are equal piecewise)
                        board[i][j].getPiece() == 'P'
                    ){
                        return false;
                    }else{
                        if(
                            board[i][j].getOwner() == my_last_move() && 
                            i + rowDiffOne > -1 && 
                            j + columnDiffOne > -1 && 
                            i + rowDiffTwo > -1 && 
                            j + columnDiffTwo > -1
                        ) return true;
                    }
                    
                }
            }
        }
    }
    return false;
}
/**
 * @brief checks for a win from either player
 * 
 * @return true - win
 * @return false - continue
 */
bool Boop::is_game_over()const{
    int rowDiffOne = 0;
    int rowDiffTwo = 0;
    int columnDiffOne = 0;
    int columnDiffTwo = 0;
    int repsOne = 0;
    int repsTwo = 0;
    const int numOfDirections = 4;
    for(int i = 0; i < numOfDirections; i++){
        if(i == 0){//checking horizontal matches first
            repsOne = 6; //outer for loop
            repsTwo = 4; //inner for loop
            columnDiffOne = 1;
            columnDiffTwo = 2;
            rowDiffOne = 0;
            rowDiffTwo = 0;
        }else if(i == 1){//checking for vertical matches
            repsOne = 4; //outer for loop
            repsTwo = 6; //inner for loop
            rowDiffOne = 1;
            rowDiffTwo = 2;
            columnDiffOne = 0;
            columnDiffTwo = 0;
        }else if(i == 2){ //top left to bottom right diagonal
            repsOne = 4; //outer for loop
            repsTwo = 4; //inner for loop
            rowDiffOne = 1;
            rowDiffTwo = 2;
            columnDiffOne = 1;
            columnDiffTwo = 2;
        }else if(i == 3){ //bottom left to top right diagonal
            repsOne = 4; //outer for loop
            repsTwo = 4; //inner for loop
            rowDiffOne = -1;
            rowDiffTwo = -2;
            columnDiffOne = 1;
            columnDiffTwo = 2;
        }
        for(int i = 0; i < repsOne; i++){
            for(int j = 0; j < repsTwo; j++){
                if( board[i][j].getPiece() == board[i + rowDiffOne][j + columnDiffOne].getPiece() && //1 and 2 are equal piecewise
                    board[i][j].getPiece() == board[i + rowDiffTwo][j + columnDiffTwo].getPiece() && //1 and 3 are equal piecewise
                    //all the spaces have an actual piece on them
                    board[i][j].isSpaceClosed() && board[i + rowDiffOne][j + columnDiffOne].isSpaceClosed() && board[i + rowDiffTwo][j + columnDiffTwo].isSpaceClosed() && 
                    board[i][j].getOwner() == board[i + rowDiffOne][j + columnDiffOne].getOwner() &&  //1 and 2 are equal owner wise
                    board[i][j].getOwner() == board[i + rowDiffTwo][j + columnDiffTwo].getOwner() &&//1 and 3 are equal owner wise
                    board[i][j].getPiece() == 'P' //if all them are planes
                ){
                    return true;
                }
            }
        }
    }
    return false;
}
/**
 * @brief Game Loop Section
 * 
 * 
 * 
 * 
 * 
 * 
 */
/**
 * Holds the 3 different make_moves functions for each type of move_type
*/
void Boop::make_move(const string& input){
    if(move_type == 0){
        // cout << RED << "HERE 0?\n" << move_type << endl << RESET;
        insert_piece(input);
    }else if(move_type == 1){
        // cout << RED << "HERE 1?\n" << RESET;
        remove3Pieces(input);
    }else if(move_type == 2){
        // cout << RED << "HERE 2?\n" << RESET;
        remove_one(input);
    }
    
    game::make_move(input);
}
/**
 * Holds the 3 different is_legal functions for each type of is_legal
*/
bool Boop::is_legal(const string& input)const{
    if(move_type == 0) return insert_is_legal(input);
    else if(move_type == 1) return remove_three_is_legal(input);
    else if(move_type == 2) return remove_one_is_legal(input);
    else return false;
}
/**
 * @brief Displays a certain message based on the move_type
 * 
 * @return string 
 */
string Boop::get_user_move()const{
    string answer;
		
    if(move_type == 0) cout << "Move Please: ";
    else if(move_type == 1){
        cout << YELLOW << BOLD <<
            "If you are able to remove pieces in accordance with the rules follow the steps below" << RESET << endl << YELLOW << 

            "\tThe 3 pieces do not have to match piecewise, only owner" << endl <<

            "\tEnter ROW, COLUMN for Each of the 3 pieces you would like removed" << endl <<

            "\tFollowed by the Direction of the Pieces" << endl << BOLD << 

            "\t\tH for Horizontal" << endl << 

            "\t\tV for Vertical" << endl << 

            "\t\tQ for Diagonal Top Left to Bottom Right" << endl << 

            "\t\tE for Diagonal Top Right to Bottom Left" << endl << 

            "\t\tExample 000102H, 001122Q" << endl << 

            RESET << endl << "Input : ";
    }
    else if(move_type == 2) {
        cout << "What is the one piece you would like to remove? Only input the coordinates (Row Column): " ;
    }
	getline(cin, answer);
	return answer;
}
/**
 * @brief Move Type Functions
 * 
 */
//Move Type 1 Functions
/**
 * @brief Move type 1
 * 
 * @param input 
 */
void Boop::insert_piece(const string& input){
    // move type 1
    //changes the owner of the selected space, and tells the piece to be inputted
    board[ int(input[0] - '0') ][ int(input[1] - '0') ].change_owner(current_player, input[2]);
    
    pieceAround(input); //makes the pieces react to each other

    //updating the avaliable pieces
    if(toupper(input[2]) == 'P'){ //large pieces
        if(board[ int(input[0] - '0') ][ int(input[1] - '0') ].getOwner() == 1){ //p1
            player1_large--;
        }else{ //p2
            player2_large--;
        }
    }else if(toupper(input[2]) == 'C'){ //small pieces
        if(board[ int(input[0] - '0') ][ int(input[1] - '0') ].getOwner() == 1){ //p1
            player1_small--;
        }else{ //p2
            player2_small--;
        }
    }

    if(three_in_row()){
        // cout << RED << "AM I EVER HERE?" << RESET;
        //move type 2
        move_type = 1;
        return;
    }
    if(no_pieces()){
        // cout << YELLOW << "AM I EVER HERE?" << RESET;
        move_type = 2;
        return;
    }


    //changes my player variable
    if(current_player == 1){
        current_player = 2;
    }else if(current_player == 2){
        current_player = 1;
    } //changes the current player

    game::make_move(input);
}
/**
 * @brief confirms if the input from the player is legal and able to to be played
 * 
 * @param input - string from the user
 * @return true - the string is valid
 * @return false - the string is invalid
 */
bool Boop::insert_is_legal(const string& input)const{
    if(input.length() != 3){
        return false; //input has to be length 3 exactly
    } 
    else if(int(input[0] - '0') > 5 || int(input[0] - '0') < 0) {  
        return false; //out of bounds row
    } 
    else if(int(input[1] - '0') > 5 || int(input[1] - '0') < 0) {
        return false; //out of bounds column
    }
    else if(toupper(input[2]) != 'P' && toupper(input[2]) != 'C') {
        return false; //not a valid piece selection
    }
    else if(board[int(input[0] - '0')][int(input[1] - '0')].isSpaceClosed()){ 
        return false; //if there is a piece on the space
    }
    else if(toupper(input[2]) == 'P' && player1_large - 1 <  0 && current_player == 1){
        return false; //if p1 does not have large pieces
    }
    else if(toupper(input[2]) == 'P' && player2_large - 1 <  0 && current_player == 2){
        return false; //if p2 does not have large pieces
    }
    else if(toupper(input[2]) == 'C' && player1_small- 1 <  0 && current_player == 1){
        return false; //if p1 does not have small pieces
    }
    else if(toupper(input[2]) == 'P' && player2_small- 1  <  0 && current_player == 2){
        return false; //if p2 does not have small pieces
    }
    else{
        return true;
    } 
}

//Move Type 2
/**
 * @brief Remove 3 Pieces in a Row, assumes the input is legal
 * 
 */
void Boop::remove3Pieces(const string& piecesRemoved){
    // cout << RED << "You are HERE?" << RESET;
    //we went through basic error checking of the input now we test for them being side by side and equal in pieces and owner
    if(toupper(piecesRemoved[6]) == 'H'){
        int leftColumn = 1000;
        int row = int(piecesRemoved[0] - '0'); //we confirmed they are on the same row, so just take the first inputted row
        for(int i = 1; i < 6; i += 2){ //finds the left most column
            if(int(piecesRemoved[i] - '0') < leftColumn) leftColumn = int(piecesRemoved[i] - '0');
        }

        //checking to make sure that all the pieces equal each other
        if( 
            // board[row][leftColumn].getPiece() == board[row][leftColumn + 1].getPiece() && //1 and 2 are equal piecewise
            // board[row][leftColumn].getPiece() == board[row][leftColumn + 2].getPiece() && //1 and 3 are equal piecewise
            //all the spaces have an actual piece on them
            board[row][leftColumn].isSpaceClosed() && board[row][leftColumn + 1].isSpaceClosed() && board[row][leftColumn + 2].isSpaceClosed() && 
            board[row][leftColumn].getOwner() == board[row][leftColumn + 1].getOwner() &&  //1 and 2 are equal owner wise
            board[row][leftColumn].getOwner() == board[row][leftColumn + 2].getOwner() //1 and 3 are equal owner wise
        ){
            if(board[row][leftColumn].getOwner() == 1){ //p1
                player1_large += 3;
            }else{ //p2
                player2_large += 3;
            }

            //if the game is continuing we must remove the pieces
            if(!is_game_over()) for(int i = 0; i < 3; i++) board[row][leftColumn + i].change_owner(0 , '\0'); 
        }
        else{ 
        //     cout << B_YELLOW << "Error, the pieces inputted either: did not match each other or were blank. Try again" << RESET << endl;
        //     remove3Pieces();
            return;
        }
    }
    //checking that the vertical pieces are all touching and matching in piece and owner
    if(toupper(piecesRemoved[6]) == 'V'){
        int upperRow = 1000;
        int column = int(piecesRemoved[1] - '0'); //since we already checked they were in the same row
        for(int i = 0; i < 5; i += 2){
            if(int(piecesRemoved[i] - '0') < upperRow) upperRow = int(piecesRemoved[i] - '0');
        }
        if( 
            // board[upperRow][column].getPiece() == board[upperRow + 1][column].getPiece() && //1 and 2 are equal
            // board[upperRow][column].getPiece() == board[upperRow + 2][column].getPiece() && //1 and 3 are equal
            //each of the spaces are closed
            board[upperRow][column].isSpaceClosed() && board[upperRow + 1][column].isSpaceClosed() && board[upperRow + 2][column].isSpaceClosed() &&
            board[upperRow][column].getOwner() ==  board[upperRow + 1][column].getOwner() && // 1 and 2 are equal owners
            board[upperRow][column].getOwner() ==  board[upperRow + 2][column].getOwner() //1 and 3 are equal owners
        ){
            if(board[upperRow][column].getOwner() == 1){ //p1
                player1_large += 3;
            }else{ //p2
                player2_large += 3;
            }

            //if the game is continuing we must remove the pieces
            if(!is_game_over()) for(int i = 0; i < 3; i++) board[upperRow + i][column].change_owner(0 , '\0'); 
        }
        else{
            // cout << B_YELLOW << "Error, the pieces inputted either: did not match each other or were blank. Try again" << RESET << endl;
            // remove3Pieces();
            return;
        }
    }
    //checking for the bottom left to to top right to left diagonal
    if(toupper(piecesRemoved[6]) == 'E'){
        //finding the bottom left piece
        int leftColumn = 1000;
        for(int i = 1; i < 6; i += 2){ //finds the left most column
            if(int(piecesRemoved[i] - '0') < leftColumn) leftColumn = int(piecesRemoved[i] - '0');
        }
        int bottomRow = -1000;
        for(int i = 0; i < 5; i += 2){ //finds the left most column
            if(int(piecesRemoved[i] - '0') > bottomRow) bottomRow = int(piecesRemoved[i] - '0');
        }
        if( 
            // board[bottomRow][leftColumn].getPiece() == board[bottomRow - 1][leftColumn + 1].getPiece() && //1 and 2 are equal
            // board[bottomRow][leftColumn].getPiece() == board[bottomRow - 2][leftColumn + 2].getPiece() && //1 and 3 are equal
            //each of the spaces are closed
            board[bottomRow][leftColumn].isSpaceClosed() && board[bottomRow - 1][leftColumn + 1].isSpaceClosed() && board[bottomRow - 2][leftColumn + 2].isSpaceClosed() &&
            board[bottomRow][leftColumn].getOwner() ==  board[bottomRow - 1][leftColumn + 1].getOwner() && // 1 and 2 are equal owners
            board[bottomRow][leftColumn].getOwner() ==  board[bottomRow - 2][leftColumn + 2].getOwner() //1 and 3 are equal owners
        ){
            if(board[bottomRow][leftColumn].getOwner() == 1){ //p1
                player1_large += 3;
            }else{ //p2
                player2_large += 3;
            }

            //if the game is continuing we must remove the pieces
            if(!is_game_over()) for(int i = 0; i < 3; i++) board[bottomRow - i][leftColumn + i].change_owner(0 , '\0'); 
        }
        else{
            // cout << B_YELLOW << "Error, the pieces inputted either: did not match each other or were blank. Try again" << RESET << endl;
            // remove3Pieces();
            return;
        }
    }
    //checking bottom right to top left
    if(toupper(piecesRemoved[6]) == 'Q'){
        int leftColumn = 1000;
        for(int i = 1; i < 6; i += 2){ //finds the left most column
            if(int(piecesRemoved[i] - '0') < leftColumn) leftColumn = int(piecesRemoved[i] - '0');
        }
        int upperRow = 1000;
        for(int i = 0; i < 5; i += 2){
            if(int(piecesRemoved[i] - '0') < upperRow) upperRow = int(piecesRemoved[i] - '0');
        }
        if( 
            // board[upperRow][leftColumn].getPiece() == board[upperRow + 1][leftColumn + 1].getPiece() && //1 and 2 are equal
            // board[upperRow][leftColumn].getPiece() == board[upperRow + 2][leftColumn + 2].getPiece() && //1 and 3 are equal
            //each of the spaces are closed
            board[upperRow][leftColumn].isSpaceClosed() && board[upperRow + 1][leftColumn + 1].isSpaceClosed() && board[upperRow + 2][leftColumn + 2].isSpaceClosed() &&
            board[upperRow][leftColumn].getOwner() ==  board[upperRow + 1][leftColumn + 1].getOwner() && // 1 and 2 are equal owners
            board[upperRow][leftColumn].getOwner() ==  board[upperRow + 2][leftColumn + 2].getOwner() //1 and 3 are equal owners
        ){
            if(board[upperRow][leftColumn].getOwner() == 1){ //p1
                player1_large += 3;
            }else{ //p2
                player2_large += 3;
            }

            //if the game is continuing we must remove the pieces
            if(!is_game_over()) for(int i = 0; i < 3; i++) board[upperRow + i][leftColumn + i].change_owner(0 , '\0'); 
        }
        else{
            // cout << B_YELLOW << "Error, the pieces inputted either: did not match each other or were blank. Try again" << RESET << endl;
            // remove3Pieces();
            return;
        }
    }

    move_type = 0;
    if(current_player == 1){
        current_player = 2;
    }else if(current_player == 2){
        current_player = 1;
    } //changes the current player
    game::make_move(piecesRemoved);
}
/**
 * @brief Checks if the input is correct for removing 3 in a row
 * 
 * @param piecesRemoved 
 * @return true - good to go and remove
 * @return false  - not
 */
bool Boop::remove_three_is_legal(const string& piecesRemoved)const{
    bool isInvalid = false;
    //if no move available for the user
    // if(piecesRemoved == "N" || piecesRemoved == "n") return;

    if(board[int(piecesRemoved[0] - '0')][int(piecesRemoved[1] - '0')].getOwner() != my_last_move()){
        isInvalid = true;
    }

    //not the correct amount of information (length)
    if(piecesRemoved.length() != 7) isInvalid = true; 

    //will loop through each of the inputted coordinate spots
    //if even one is out of bounds it is invalid
    for(int i = 0; i < 6; i++){
        if(int(piecesRemoved[i] - '0') > 5 || int(piecesRemoved[i] - '0') < 0) isInvalid = true; 
    }

    //if the Direction given is not one of the 3 options
    if(toupper(piecesRemoved[6]) != 'H' && toupper(piecesRemoved[6]) != 'V' && toupper(piecesRemoved[6]) != 'E' && toupper(piecesRemoved[6]) != 'Q'){
        isInvalid = true;
    }
    //checking for the pieces being Horizontal
    if(toupper(piecesRemoved[6]) == 'H'){
        //checks to make sure that all on the same row, since they need to be for it to be horizontal
        if(int(piecesRemoved[0] - '0') != int(piecesRemoved[2] - '0') || int(piecesRemoved[0] - '0') != int(piecesRemoved[4] - '0')){
            isInvalid = true;
        }
    }
    //checking for the same column for Vertical
    if(toupper(piecesRemoved[6]) == 'V'){
        //checks to make sure that all on the same row, since they need to be for it to be horizontal
        if(int(piecesRemoved[1] - '0') != int(piecesRemoved[3] - '0') || int(piecesRemoved[1] - '0') != int(piecesRemoved[5] - '0')){
            isInvalid = true;
        }
    }
    //for top left to bottom right
    if(toupper(piecesRemoved[6]) == 'Q'){
        //check to make sure that they are all top left to bottom right
        int leftColumn = 1000;
        for(int i = 1; i < 6; i += 2){ //finds the left most column
            if(int(piecesRemoved[i] - '0') < leftColumn) leftColumn = int(piecesRemoved[i] - '0');
        }
        int upperRow = 1000;
        for(int i = 0; i < 5; i += 2){
            if(int(piecesRemoved[i] - '0') < upperRow) upperRow = int(piecesRemoved[i] - '0');
        }
        if( 
            //each of the spaces are closed
            !(board[upperRow][leftColumn].isSpaceClosed() && board[upperRow + 1][leftColumn + 1].isSpaceClosed() && board[upperRow + 2][leftColumn + 2].isSpaceClosed() &&
            board[upperRow][leftColumn].getOwner() ==  board[upperRow + 1][leftColumn + 1].getOwner() && // 1 and 2 are equal owners
            board[upperRow][leftColumn].getOwner() ==  board[upperRow + 2][leftColumn + 2].getOwner()) //1 and 3 are equal owners
        ){
            isInvalid = true;
        }
    }
    //top right to bottom left
    if(toupper(piecesRemoved[6]) == 'E'){
        //check to make sure that they are all top left to bottom right
        int leftColumn = 1000;
        for(int i = 1; i < 6; i += 2){ //finds the left most column
            if(int(piecesRemoved[i] - '0') < leftColumn) leftColumn = int(piecesRemoved[i] - '0');
        }
        int bottomRow = -1000;
        for(int i = 0; i < 5; i += 2){ //finds the left most column
            if(int(piecesRemoved[i] - '0') > bottomRow) bottomRow = int(piecesRemoved[i] - '0');
        }
        if( 
            // board[bottomRow][leftColumn].getPiece() == board[bottomRow - 1][leftColumn + 1].getPiece() && //1 and 2 are equal
            // board[bottomRow][leftColumn].getPiece() == board[bottomRow - 2][leftColumn + 2].getPiece() && //1 and 3 are equal
            //each of the spaces are closed
            !(board[bottomRow][leftColumn].isSpaceClosed() && board[bottomRow - 1][leftColumn + 1].isSpaceClosed() && board[bottomRow - 2][leftColumn + 2].isSpaceClosed() &&
            board[bottomRow][leftColumn].getOwner() ==  board[bottomRow - 1][leftColumn + 1].getOwner() && // 1 and 2 are equal owners
            board[bottomRow][leftColumn].getOwner() ==  board[bottomRow - 2][leftColumn + 2].getOwner()) //1 and 3 are equal owners
        ){
            isInvalid = true;
        }
    }
    //we went through basic error checking of the input now we test for them being side by side and equal in pieces and owner
    if(isInvalid) return false;
    else return true;
}

//Move Type 3
/**
 * @brief Removes the piece at the coordinate. assumes that the move given is legal
 * 
 * @param input 
 */
void Boop::remove_one(const string& input){

    board[ int(input[0] - '0') ][ int(input[1] - '0') ].change_owner(0 , '\0');

    move_type = 0;

    if(current_player == 1){
        player1_large++;
        current_player = 2;
    }else if(current_player == 2){
        player2_large++;
        current_player = 1;
    } //changes the current player

    game::make_move(input);
}
/**
 * @brief Checks if the input is correct for removing 1 piece
 * 
 * @param input 
 * @return true - good to go
 * @return false - not
 */
bool Boop::remove_one_is_legal(const string& input)const{
    if(
        int(input[0] - '0') > -1 && 
        int(input[0] - '0') < 6 && 
        int(input[1] - '0') > -1 && 
        int(input[1] - '0') < 6
    ){
        return true;
    }
    return false;
}

/**
 * @brief Computer Section
 * 
 * 
 * 
 * 
 * 
 * 
 */

/**
 * @brief Makes a queue of every available move to be made
 * 
 * @param moves - the queue that will hold each move
 */
void Boop::compute_moves(std::queue<std::string>& moves) const{
    if(move_type == 0){ //put a single piece on the board
        for(int i = 0; i < 6; i++){
            for (int j = 0; j < 6; j++){
                if(!board[i][j].isSpaceClosed()){
                    string buffer1 , buffer2;
                    //adding the coordinates
                    buffer1 += to_string(i);
                    buffer1 += to_string(j);
                    buffer2 = buffer1;
                    //adding either cat or kitten
                    buffer1 += "P";
                    buffer2 += "C";
                    // cout << "Buffer 1: " << buffer1 << endl << "Buffer 2: " << buffer2 << endl;
                    if(insert_is_legal(buffer1) && player2_large > 0) {
                        // cout << "WHAT" << endl;
                        // cout << buffer1 << endl;
                        moves.push(buffer1);
                    }
                    if(insert_is_legal(buffer2) && player2_small > 0){
                        // cout << "What" << endl;
                        moves.push(buffer2);
                    }
                }
            }
        }
    }else if(move_type == 1){ //remove 3 pieces
        // if(three_in_row()){ //just if there is any three in a row on the board
            int rowDiffOne = 0;
            int rowDiffTwo = 0;
            int columnDiffOne = 0;
            int columnDiffTwo = 0;
            int repsOne = 0;
            int repsTwo = 0;
            int numOfDirections = 4;
            for(int direction = 0; direction < numOfDirections; direction++){
                if(direction == 0){//checking horizontal matches first
                    repsOne = 6; //outer for loop
                    repsTwo = 4; //inner for loop
                    columnDiffOne = 1;
                    columnDiffTwo = 2;
                    rowDiffOne = 0;
                    rowDiffTwo = 0;
                }else if(direction == 1){//checking for vertical matches
                    repsOne = 4; //outer for loop
                    repsTwo = 6; //inner for loop
                    rowDiffOne = 1;
                    rowDiffTwo = 2;
                    columnDiffOne = 0;
                    columnDiffTwo = 0;
                }else if(direction == 2){ //top left to bottom right diagonal
                    repsOne = 4; //outer for loop
                    repsTwo = 4; //inner for loop
                    rowDiffOne = 1;
                    rowDiffTwo = 2;
                    columnDiffOne = 1;
                    columnDiffTwo = 2;
                }else if(direction == 3){ //bottom left to top right diagonal
                    repsOne = 4; //outer for loop
                    repsTwo = 4; //inner for loop
                    rowDiffOne = -1;
                    rowDiffTwo = -2;
                    columnDiffOne = 1;
                    columnDiffTwo = 2;
                }
                for(int i = 0; i < repsOne; i++){
                    for(int j = 0; j < repsTwo; j++){
                        if(
                            //board[i][j].getPiece() == board[i + rowDiffOne][j + columnDiffOne].getPiece() && //1 and 2 are equal piecewise
                            //board[i][j].getPiece() == board[i + rowDiffTwo][j + columnDiffTwo].getPiece() && //1 and 3 are equal piecewise
                            //all the spaces have an actual piece on them
                            board[i][j].isSpaceClosed() && board[i + rowDiffOne][j + columnDiffOne].isSpaceClosed() && board[i + rowDiffTwo][j + columnDiffTwo].isSpaceClosed() && 
                            board[i][j].getOwner() == board[i + rowDiffOne][j + columnDiffOne].getOwner() &&  //1 and 2 are equal owner wise
                            board[i][j].getOwner() == board[i + rowDiffTwo][j + columnDiffTwo].getOwner()//1 and 3 are equal owner wise
                        ){
                            string buffer1;
                            buffer1 += to_string(i);
                            buffer1 += to_string(j);
                            buffer1 += to_string(i + rowDiffOne);
                            buffer1 += to_string(i + columnDiffOne);
                            buffer1 += to_string(i + rowDiffTwo);
                            buffer1 += to_string(i + columnDiffTwo);

                            //adding the direction command
                            if(direction == 0) buffer1 += "H";
                            else if(direction == 1) buffer1 += "V";
                            else if(direction == 2) buffer1 += "Q";
                            else if(direction == 3) buffer1 += "E";


                            if(remove_three_is_legal(buffer1)) moves.push(buffer1);
                        }
                    }
                }
            // }
        }
    }else if(move_type == 2){ //remove 1 piece
        for(int i = 0; i < 6; i++){
            for (int j = 0; j < 6; j++){
                if(board[i][j].isSpaceClosed()){
                    string buffer1;
                    buffer1 += to_string(i);
                    buffer1 += to_string(j);


                    if(remove_one_is_legal(buffer1)) moves.push(buffer1);
                }
            }
        }
    }
}
/**
 * @brief Gives a score to each board depending on the pieces on the board
 * 
 * @return int - the score, negative is good for player 1 (Human) positive is good for player 2 (Computer)
 */
int Boop::evaluate()const{
    // Positive numbers returned from this function are good for the computer.
    // computer is player 2
    int total = 0;

    const int single_plane_on = 16;
    const int single_plane_off = 12;
    const int single_heli_on = 2;
    const int single_heli_off = 10;

    const int double_plane = 6;
    const int double_heli = 4;
    const int double_mixed = 3;

    const int triple_plane = 10000; //this is literal win, must be higher than anything
    const int triple_heli = 5; 
    const int triple_mixed = 5;

    //taking all the pieces off the board
    total -= player1_large * single_plane_off; //# of planes * points for plane off board for player 1
    total -= player1_small * single_heli_off; //# of helis * points for heli off board for player 1
    total += player2_large * single_plane_off; //# of planes * points for plane off board for player 2
    total += player2_small * single_heli_off; //# of helis * points for heli off board for player 2


    //checking for singles on the board
    {
        for(int i = 0; i < 6; i++){
            for (int j = 0; j < 6; j++){
                if(board[i][j].getPiece() == 'P'){ //single plane
                    if(board[i][j].getOwner() == 1) total -= single_plane_on; //human
                    if(board[i][j].getOwner() == 2) total += single_plane_on; //computer
                }
                else if(board[i][j].getPiece() == 'C'){ //single helicopter
                    if(board[i][j].getOwner() == 1) total -= single_heli_on; //human
                    if(board[i][j].getOwner() == 2) total += single_heli_on; //computer
                }
            }
        }   
    }
    //checking for 3 in a row for, planes, helicopters, and then mixed
    {
    //three in a row
    int rowDiffOne = 0;
    int rowDiffTwo = 0;
    int columnDiffOne = 0;
    int columnDiffTwo = 0;
    int repsOne = 0;
    int repsTwo = 0;
    int numOfDirections = 4;
    for(int i = 0; i < numOfDirections; i++){
        if(i == 0){//checking horizontal matches first
            repsOne = 6; //outer for loop
            repsTwo = 4; //inner for loop
            columnDiffOne = 1;
            columnDiffTwo = 2;
            rowDiffOne = 0;
            rowDiffTwo = 0;
        }else if(i == 1){//checking for vertical matches
            repsOne = 4; //outer for loop
            repsTwo = 6; //inner for loop
            rowDiffOne = 1;
            rowDiffTwo = 2;
            columnDiffOne = 0;
            columnDiffTwo = 0;
        }else if(i == 2){ //top left to bottom right diagonal
            repsOne = 4; //outer for loop
            repsTwo = 4; //inner for loop
            rowDiffOne = 1;
            rowDiffTwo = 2;
            columnDiffOne = 1;
            columnDiffTwo = 2;
        }else if(i == 3){ //bottom left to top right diagonal
            repsOne = 4; //outer for loop
            repsTwo = 4; //inner for loop
            rowDiffOne = -1;
            rowDiffTwo = -2;
            columnDiffOne = 1;
            columnDiffTwo = 2;
        }
        for(int i = 0; i < repsOne; i++){
            for(int j = 0; j < repsTwo; j++){
                if(
                    //board[i][j].getPiece() == board[i + rowDiffOne][j + columnDiffOne].getPiece() && //1 and 2 are equal piecewise
                    //board[i][j].getPiece() == board[i + rowDiffTwo][j + columnDiffTwo].getPiece() && //1 and 3 are equal piecewise
                    //all the spaces have an actual piece on them
                    board[i][j].isSpaceClosed() && board[i + rowDiffOne][j + columnDiffOne].isSpaceClosed() && board[i + rowDiffTwo][j + columnDiffTwo].isSpaceClosed() && 
                    board[i][j].getOwner() == board[i + rowDiffOne][j + columnDiffOne].getOwner() &&  //1 and 2 are equal owner wise
                    board[i][j].getOwner() == board[i + rowDiffTwo][j + columnDiffTwo].getOwner()//1 and 3 are equal owner wise
                ){
                    if(
                        //if there is 3 Planes in a Row
                        board[i][j].getPiece() == board[i + rowDiffOne][j + columnDiffOne].getPiece() && //1 and 2 are equal piecewise
                        board[i][j].getPiece() == board[i + rowDiffTwo][j + columnDiffTwo].getPiece() && //1 and 3 are equal piecewise)
                        board[i][j].getPiece() == 'P'
                    ){
                        if(board[i][j].getOwner() == 1){ //3 in a row planes for the human
                            total -= triple_plane;
                        }else if(board[i][j].getOwner() == 2){ //3 in a row planes for the computer
                            total += triple_plane;
                        }
                    }
                    else if(
                        //if there are 3 helis in a row
                        board[i][j].getPiece() == board[i + rowDiffOne][j + columnDiffOne].getPiece() && //1 and 2 are equal piecewise
                        board[i][j].getPiece() == board[i + rowDiffTwo][j + columnDiffTwo].getPiece() && //1 and 3 are equal piecewise)
                        board[i][j].getPiece() == 'C'
                    ){
                        if(board[i][j].getOwner() == 1){ //3 in a row planes for the human
                            total -= triple_heli;
                        }else if(board[i][j].getOwner() == 2){ //3 in a row planes for the computer
                            total += triple_heli;
                        }
                    }
                    else{
                        //for anything that has a mix of planes and helicopters
                        if(board[i][j].getOwner() == 1){ //mixed 3 in a row for the human
                            total -= triple_mixed;
                        }else if(board[i][j].getOwner() == 2){ //mixed 3 in a row for the computer
                            total += triple_mixed;
                        }
                    }
                    
                }
            }
        }
    }
    }
    //checking for any possible adjacent pieces
   {
    //checking for adjacent
    //two in a row
    int rowDiffOne = 0;
    int rowDiffTwo = 0;
    int columnDiffOne = 0;
    int columnDiffTwo = 0;
    int repsOne = 0;
    int repsTwo = 0;
    int numOfDirections = 4;
    for(int direction = 0; direction < 3; direction++){
        if(direction == 0){//horizontal
            rowDiffOne = 0;
            columnDiffOne = 1;
            repsOne = 6; //rows
            repsTwo = 5; //columns
        }else if(direction == 1){ //vertical
            rowDiffOne = 1;
            columnDiffOne = 0;
            repsOne = 5; //rows
            repsTwo = 6; //columns
        }else if(direction == 2){ //top left to bottom right
            rowDiffOne = 1;
            columnDiffOne = 1;
            repsOne = 5; //rows
            repsTwo = 5; //columns
        }
        for(int i = 0; i < repsOne; i++){
            for(int j = 0; j < repsTwo; j++){
                if(
                    (board[i][j].getOwner() == board[i + rowDiffOne][j + columnDiffOne].getOwner()) && //same owner
                    board[i][j].isSpaceClosed() && board[i + rowDiffOne][j + columnDiffOne].isSpaceClosed() //have some piece on it
                ){
                    if(board[i][j].getPiece() == 'P' &&  board[i + rowDiffOne][j + columnDiffOne].getPiece() == 'P'){ //both are plane
                        if(board[i][j].getOwner() == 1){ //player 1
                            total -= double_plane;
                        }else{ //player 2
                            total += double_plane;
                        }
                    }else if(board[i][j].getPiece() == 'C' &&  board[i + rowDiffOne][j + columnDiffOne].getPiece() == 'C'){ //both are heli
                        if(board[i][j].getOwner() == 1){ //player 1
                            total -= double_heli;
                        }else{ //player 2
                            total += double_heli;
                        }
                    }else{
                        if(board[i][j].getOwner() == 1){ //player 1
                            total -= double_mixed;
                        }else{ //player 2
                            total += double_mixed;
                        }
                    }
                }
            }
        }
    }
    //top right to bottom left
    for(int i = 1; i < 6; i++){
        for(int j = 0; j < 5; j++){
            if(
                (board[i][j].getOwner() == board[i - 1 ][j + 1].getOwner()) && //same owner
                board[i][j].isSpaceClosed() && board[i - 1 ][j + 1].isSpaceClosed() //have some piece on it
            ){
                if(board[i][j].getPiece() == 'P' &&  board[i + rowDiffOne][j + columnDiffOne].getPiece() == 'P'){ //both are plane
                    if(board[i][j].getOwner() == 1){ //player 1
                        total -= double_plane;
                    }else{ //player 2
                        total += double_plane;
                    }
                }else if(board[i][j].getPiece() == 'C' &&  board[i + rowDiffOne][j + columnDiffOne].getPiece() == 'C'){ //both are heli
                    if(board[i][j].getOwner() == 1){ //player 1
                        total -= double_heli;
                    }else{ //player 2
                        total += double_heli;
                    }
                }else{
                    if(board[i][j].getOwner() == 1){ //player 1
                        total -= double_mixed;
                    }else{ //player 2
                        total += double_mixed;
                    }
                }
            }
        }
    }

    //with gap
    for(int i = 0; i < numOfDirections; i++){
        if(i == 0){//checking horizontal matches first
            repsOne = 6; //outer for loop
            repsTwo = 4; //inner for loop
            columnDiffOne = 1;
            columnDiffTwo = 2;
            rowDiffOne = 0;
            rowDiffTwo = 0;
        }else if(i == 1){//checking for vertical matches
            repsOne = 4; //outer for loop
            repsTwo = 6; //inner for loop
            rowDiffOne = 1;
            rowDiffTwo = 2;
            columnDiffOne = 0;
            columnDiffTwo = 0;
        }else if(i == 2){ //top left to bottom right diagonal
            repsOne = 4; //outer for loop
            repsTwo = 4; //inner for loop
            rowDiffOne = 1;
            rowDiffTwo = 2;
            columnDiffOne = 1;
            columnDiffTwo = 2;
        }
        for(int i = 0; i < repsOne; i++){
            for(int j = 0; j < repsTwo; j++){
                if(
                    //board[i][j].getPiece() == board[i + rowDiffOne][j + columnDiffOne].getPiece() && //1 and 2 are equal piecewise
                    //board[i][j].getPiece() == board[i + rowDiffTwo][j + columnDiffTwo].getPiece() && //1 and 3 are equal piecewise
                    //all the spaces have an actual piece on them
                    board[i][j].isSpaceClosed() && board[i + rowDiffTwo][j + columnDiffTwo].isSpaceClosed() && 
                    board[i][j].getOwner() == board[i + rowDiffTwo][j + columnDiffTwo].getOwner()//1 and 3 are equal owner wise
                ){
                    if(board[i][j].getPiece() == 'P' &&  board[i + rowDiffOne][j + columnDiffOne].getPiece() == 'P'){ //both are plane
                        if(board[i][j].getOwner() == 1){ //player 1
                            total -= double_plane;
                        }else{ //player 2
                            total += double_plane;
                        }
                    }else if(board[i][j].getPiece() == 'C' &&  board[i + rowDiffOne][j + columnDiffOne].getPiece() == 'C'){ //both are heli
                        if(board[i][j].getOwner() == 1){ //player 1
                            total -= double_heli;
                        }else{ //player 2
                            total += double_heli;
                        }
                    }else{
                        if(board[i][j].getOwner() == 1){ //player 1
                            total -= double_mixed;
                        }else{ //player 2
                            total += double_mixed;
                        }
                    }
                }
            }
        }
        //bottom right to top left with gap 
        for(int i = 2; i < 6; i ++){
            for(int j = 0; j < 4; j++){
                // cout <<  "First: " << i << j << endl;
                // cout << "Second: " << i - 2 << j + 2 << endl;
                if(
                (board[i][j].getOwner() == board[i - 2 ][j + 2].getOwner()) && //same owner
                board[i][j].isSpaceClosed() && board[i - 2][j + 2].isSpaceClosed() //have some piece on it
                ){
                    if(board[i][j].getPiece() == 'P' &&  board[i + rowDiffOne][j + columnDiffOne].getPiece() == 'P'){ //both are plane
                        if(board[i][j].getOwner() == 1){ //player 1
                            total -= double_plane;
                        }else{ //player 2
                            total += double_plane;
                        }
                    }else if(board[i][j].getPiece() == 'C' &&  board[i + rowDiffOne][j + columnDiffOne].getPiece() == 'C'){ //both are heli
                        if(board[i][j].getOwner() == 1){ //player 1
                            total -= double_heli;
                        }else{ //player 2
                            total += double_heli;
                        }
                    }else{
                        if(board[i][j].getOwner() == 1){ //player 1
                            total -= double_mixed;
                        }else{ //player 2
                            total += double_mixed;
                        }
                    }
                }
            }
        }
    }
   }

    return total;
}