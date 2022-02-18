/*
Scoreboard logic + Text file from Assignment00

Match starting letter of prospective word to previous word, and then consecutively until end of previous word to check if eligible word
Starting player is random (use random number gen)
if both players pass, new alphabet
if both players pass twice in a row (keep back and forth counter) the game ends

pseudocode to check validity of new word (new)
Precondition: prev is a character array of previous valid word, new is character array prospective new word
for (int i=0; i<sizeof prev;i++)
{
    if (new[0]==prev[i])
    {
        int j = i;
        int k = 0;
        while ((prev[j]!=null) && (new[k]==prev[j])
        {
            j++;
            k++;
        }
        if (j==sizeof prev)
         word is valid
    }
}

Flow of game:
Randomly select input.txt and player1
Display first line to player1, select 1 character and require prev = just that character
word is sent to server, if valid, score updates, player1 gets both player scores from server, then player2 is shown the line from input.txt and so on

During player turn, server sends:
-> input.txt line 1
-> player score
-> other player score
-> Used words
-> prev requirements

During player sending word, server checks
-> received word validity
-> whether word is used
-> assign score as per input.txt (should be based on length)
if invalid
-> penalise for invalid word, request valid entry
if used
-> inform used, penalise
each player gets 4 minutes to enter a word
timer is reset when a word is sent to the server
if player resets the timer 3 times in one turn server considers it a pass
when a new valid word is found, it needs to be added to input.txt and scored accordingly
in singleplayer the server is only allowed to use input.txt words, not dictionary.txt
*/