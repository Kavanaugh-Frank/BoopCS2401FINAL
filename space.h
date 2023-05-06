#ifndef SPACE_H
#define SPACE_H
#include <string>
#include <iostream>
#include <iomanip>
#include "colors.h"

class Space{
    public:
        Space(){
            current_owner = 0; //1 is P1
            piece = '\0'; //there is no piece when \0 is there, P for plane and C for Chopper
        };
        bool isSpaceClosed()const{
            if(piece == '\0') return false;
            else return true;
        }
        void change_owner(int new_owner, char pieceType){
            current_owner = new_owner;
            piece =  toupper(pieceType); //makes it uniform with the checks I perform, which are all uppercase
        }
        /**
         * @brief Get the Color for the current player of the space
         * 
         * @return std::string - string of the color
         */
        std::string getColor()const{
            if(current_owner == 1){
                return RED;
            }else{
                return BLUE;
            }
        }
        /**
         * @brief Get the Owner bool, true for p1
         * 
         * @return true 
         * @return false 
         */
        int getOwner()const{return current_owner;}
        char getPiece()const{return piece;}
        /**
         * @brief Returns the top section of current piece in the space
         * 
         * @return std::string 
         */
        std::string charTop()const{
            if(piece == 'P'){
                return "               "; //plane
            }else if(piece == 'C'){
                return("   -+-         "); //chopper
            }else{
                return "               "; //empty spot
            } 
        }
        /**
         * @brief Returns the middle section of the current piece in the space
         * 
         * @return std::string 
         */
        std::string charMiddle()const{
            if(piece == 'P'){
                return("    ___|___    "); //plane
            }else if(piece == 'C'){
                return("   ( )xxxx@    "); //chopper
            }else{
                return "               ";
            }   
        }
        /**
         * @brief Returns the bottom section of the current piece in the space
         * 
         * @return std::string 
         */
        std::string charBottom()const{
            if(piece == 'P'){
                return("  *---o0o---*  "); //plane
            }else if(piece == 'C'){
                return("   *--         "); //chopper
            }else{
                return "               "; //empty spot
            } 
        }

    private:
        int current_owner; //1 for RED(P1) and 2 for BLUE(P2)
        char piece; //P for Plane, C for Chopper, and '\0' for None

};


#endif