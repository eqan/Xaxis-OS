#!/bin/bash

source gui.sh

#	*** Documentation ***
#		* This program would be divided into following modules:
#			* Global Variables
#			* Starting Functions
#			* Game_Play Functions
#			* Win/Lose determining Functions
#			* Utility Functions
#



#	*** Global Variables Start ***
#	Function: Variables to store data that will be used in between the program

current_user=""
total_rounds_won=0
total_rounds_per_mode=3
current_round=0
total_chances=6
current_chances=$total_chances
words_list=()
word=""
puzzle_word=()
current_score=0
current_mode=1
current_mode_name='Easy'
total_rounds=0
used_words=()


#	*** Global Variables End ***

#	*** Starting Functions Start ***
#	Function: Functions necessary for the initialzation of the components of the game
function game_mode_menu()
{
	local -i option
	mapfile option <temp_difficulty
	if [ $option -eq 5 ]
	then exit
	fi
	initializer_function $option
}

# <summary>
#	* Player Selects difficulty level of the puzzle
#	* A random word is selected from the list
#	* Program creates a copy of that word and makes a puzzle out of it
#	* Gameplay begins
# </summary>

function initializer_function()
{
	determine_current_mode $1
	select_game_mode $1
	pick_random_word_from_list
	create_puzzle_string
	store_global_variables
	game_play
}


# <summary>
#	* Creates a log list where every used word is stored
#	* Necessary for always allowing new words to flow
# </summary>
function check_if_word_used()
{
	if [[ ${#used_words[@]} == 0 ]]
	then
		used_words[0]=$word
		return
	fi
	for(( i=0; i< ${#used_words[@]}; i++ ))	
	do
		if [[ $word == ${used_words[i]} ]]
		then
			pick_random_word_from_list
		fi
	done
	(( i++ ))
	used_words[i]=$word
	return
}


# <summary>
#	* A random word is selected from the list
#	* Checks if the word is just blank, it then recalls the function again
#	* Converts that word from string to array
# </summary>
function pick_random_word_from_list()
{
	list_size=${#words_list[@]}
	word=${words_list[$(( ( RANDOM % $list_size + 1 )))]}
	check_if_word_used
	if [[ $word =~ ^[[:blank:]]+$ ]]  || [[ ! $word = *[!\ ]* ]]
	then
		pick_random_word_from_list
	fi
	convert_string_to_array
}

# <summary>
#	* Creates puzzle from a word, only alphabets would be filled blank
# </summary>
function create_puzzle_string()
{
	puzzle_word=()
	local status_letter_found=false
	local -i number_of_blank_spaces=0
	for ((i=0; i<${#word[@]}; i++))
	do
		for letter in {a..z}
			do
			if [[ $letter == ${word[i]} ]]
			then
				status_letter_found=true
				puzzle_word[i]='_'
				(( number_of_blank_spaces++ ))
				break
			else
				status_letter_found=false
			fi
		done
		if [[ $status_letter_found == false ]]
		then
			puzzle_word[i]=${word[i]}
		fi
		status_letter_found=false
	done
	if (( $number_of_blank_spaces <= 1 ))
	then
		initializer_function
	fi
}



#	*** Starting Functions End ***


#	*** Game_Play  Functions Start ***
#	Function: Functions necessary for the initialzation of the components of the game


# <summary>
#	* Prompts a status bar at the top indicating the current puzzle word, current chances, currrent difficulty level and current round
#	* Gets alphabet fills the puzzle if the alphabet matches, or deducts chances
#	* Checks if the word has any blank spaces left which are yet to be filled
# </summary>
function game_play()
{
	local -i isexit
	mapfile isexit < temp_difficulty
	if [ $isexit -eq 5 ]
	then 
		exit
	fi
	game
	mapfile -t userin < userinput
	get_input $userin
	check_chances
	check_if_blank_spaces_remain
}

# <summary>
#	* Reads the input and searches from the word
#	* If the input matches the character from the word, It will reveal that character from the puzzle 
#	* Else death chances increase
# </summary>
function get_input()
{
	local find_status=false
	for (( i=0; i<${#word[@]}; i++ ))
	do
		if [[ ${word[i]} == $1 ]]
		then
			find_status=true
			puzzle_word[i]=$1
		fi
	done
	if [[ $find_status == false ]]
	then
		printf "Oops chance deducted!\n"
		(( current_chances-- ))
	fi
	store_global_variables
}


# <summary>
#	* Checks if chances are below 0
#	* If yes then it will iterate to the next round, and no marks would be given
#	* Else it will do nothing 
# </summary>
function check_chances()
{
	if (( $current_chances < 0 ))
	then
		prompt 'OOPS, You Are Hanged'
		iterate_to_next_round
	fi
}

# <summary>
#	* Checks if any blank space remains in the puzzle
#	* If yes it will resume gameplay
#	* If no then it will check if the puzzle is correctly solved or incorrectly
# </summary>
function check_if_blank_spaces_remain()
{
	if [[ ${puzzle_word[@]} =~ "_" ]]
	then
		game_play
	else
		(( total_rounds_won++ ))
		add_current_score
		iterate_to_next_round
	fi
}

# <summary>
#	* Selects game difficulty from user/upon the difficulty upgrade by the program
#	* At the selected difficulty it imports the data list from the files
#	* Bonus: It can capture custom data from custom list if user has his/her own word list
# </summary>
function select_game_mode()
{
	case $1 in 
		1)
			total_rounds=$(( $total_rounds_per_mode * 3 ))
			mapfile -t words_list < Easy.txt
			current_mode=1
			break
			;;
		2)
			total_rounds=$(( $total_rounds_per_mode * 2 ))
			mapfile -t words_list < Medium.txt
			current_mode=2
			break
			;;
		3)
			total_rounds=$total_rounds_per_mode
			mapfile -t words_list < Hard.txt
			current_mode=3
			break
			;;
		4)
			total_rounds=$total_rounds_per_mode
			mapfile -t words_list < Custom.txt
			current_mode=4
			break
			;;
		*)
			printf "Option out of bounds, Please try again!\n"
			game_mode_menu
			;;
	esac
}


#	*** Game_Play  Functions End ***


#	*** Win/Lose determining  Functions Start ***
#	Function: Functions to determine the winning/losing results of the player


# <summary>
#	* A new puzzle will be assigned if a person has completed a round
#	* Difficulty level will be upgraded if a person has completed minimum set of rounds
#	* Difficulty level will not be upgraded if the win to lose ratio of the rounds is smaller than 50%
# </summary>
function iterate_to_next_round()
{
	(( current_round++ ))
	if (( $current_round >= total_rounds_per_mode ))
	then
		current_round=0
		check_if_should_be_promoted_to_next_mode
		if [[ $? == 1 ]]
		then
			prompt 'Congratulation promoted to next Difficulty!'
			(( current_mode++ ))
			if (( $current_mode >= 3 ))
			then
				results
			fi
		else
			prompt 'Win percentage below 50, thus can''t be promoted to next Difficulty!'
		fi
	fi
	current_chances=$total_chances
	word_found=false
	initializer_function $current_mode
}

# <summary>
#	* Adds score according to the difficulty level
# </summary>
function add_current_score()
{
	case $current_mode in 
		1)
			(( current_score+=10 ))
			break
			;;
		2)
			(( current_score+=20 ))
			break
			;;
		3)
			(( current_score+=30 ))
			break
			;;
		*)
			(( current_score+=15 ))
			break
			;;
	esac
}

# <summary>
#	* Checks if the difficulty level is to be upgraded or not
#	* 50% and above win to lost ratio, the difficulty level will be upgraded
# </summary>
function check_if_should_be_promoted_to_next_mode()
{
	local -i percentage 
	percentage=$(( ( (total_rounds_won + 1) * 100 ) / (total_rounds_per_mode + 1) ))
	if (( $percentage >= 50 ))
	then
		return 1
	fi
	return 0
}



#	*** Win/Lose determining  Functions End ***


#	*** Utility  Functions Start ***
#	Function: Helping functions assisting in the program

function convert_string_to_array()
{
	string=$word
	word=()
	for ((i=0; i<${#string}; i++))
	do
		word[$i]=${string:$i:1}
	done
}

function determine_current_mode()
{
	case $1 in 
		1)
			current_mode_name="Easy"
			break
			;;
		2)
			current_mode_name="Medium"
			break
			;;
		3)
			current_mode_name="Hard"
			break
			;;
		4)
			current_mode_name="Custom"
			break
			;;
	esac
}

function results()
{
	mapfile usr_name <temp_name
	prompt "Total Score: ${current_score} | Total Rounds Won: ${total_rounds_won} ${total_rounds}"
	echo "$usr_name -> ${current_score}" >> HighScores.txt
	main
}

function store_global_variables()
{
	echo "${current_user}" > TempData
	echo "${current_chances}" >> TempData
	echo "${current_round}" >> TempData
	echo "${current_score}" >> TempData
	echo "${current_mode_name}" >> TempData
	echo "${puzzle_word[@]}" >> TempData
	echo "${word[@]}" >> TempData
	echo "${total_rounds_won}" >> TempData 
	echo "${total_rounds_per_mode}" >> TempData
	echo "${total_rounds}" >> TempData
}

#	*** Utility  Functions End ***

main
