//Author: Drew Nguyen
/*
Responsibilities: Client implementation and Client-server architecture
        Reformatted handling of expressions and functions. Made code a little more clear and legible.
        Worked hard on trying to figure out certain bugs in terms of computer-client turns
        Drew worked along side adam to figure out how players would be added along side the computer player
*/

void minusPlayerScore(struct Player new_player, int num)
{
    new_player.score += num;
}

void addPlayerScore(struct Player new_player)
{
    size_t length = nnew;
    if (length == 3 || length == 4)
    {
        new_player.score += 1;
    }
    if (length == 5)
    {
        new_player.score += 2;
    }
    if (length == 6)
    {
        new_player.score += 3;
    }
    if (length == 7)
    {
        new_player.score += 4;
    }
    if (length >= 8)
    {
        new_player.score += 11;
    }
}

void minusPlayerScore(struct Player new_player, int num)
{
    new_player.score += num;
}

void addPlayerScore(struct Player new_player)
{
    size_t length = nnew;
    if (length == 3 || length == 4)
    {
        new_player.score += 1;
    }
    if (length == 5)
    {
        new_player.score += 2;
    }
    if (length == 6)
    {
        new_player.score += 3;
    }
    if (length == 7)
    {
        new_player.score += 4;
    }
    if (length >= 8)
    {
        new_player.score += 11;
    }
}

void addComputerScore(struct Computer new_computer)
{
    size_t length = nnew;
    if (length == 3 || length == 4)
    {
        new_computer.score += 1;
    }
    if (length == 5)
    {
        new_computer.score += 2;
    }
    if (length == 6)
    {
        new_computer.score += 3;
    }
    if (length == 7)
    {
        new_computer.score += 4;
    }
    if (length >= 8)
    {
        new_computer.score += 11;
    }
}