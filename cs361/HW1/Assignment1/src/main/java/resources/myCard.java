/**
 * Created by dominic on 1/19/16.
 * For our deck
 * Modified by Alex
 */
package resources;
public class myCard {
    private int rank;
    private char suit;
    public boolean hasBeenPlayed;
    public myCard()
    {
        this.rank = 0;
        this.suit = ' ';
        hasBeenPlayed = false;
    }
    public myCard(int x, char y)
    {
        this.setRank(x);
        this.setSuit(y);
        this.hasBeenPlayed = false;
    }


    public void setRank(int x){
        this.rank = x;
    }
    public void setSuit(char x){
        this.suit = x;
    }
    public int getRank(){
        return this.rank;
    }
    public char getSuit(){
        return this.suit;
    }

    // return 1 if less, 2 if greater, 0 if equal
    public int compareRank(myCard otherCard)
    {
        if (this.getRank() > otherCard.getRank()){
            return 2;
        } else if (this.getRank() < otherCard.getRank()) {
            return 1;
        } else if (this.getRank() == otherCard.getRank()) {
            return 0;
        }
        return 3;
    }

    public boolean compareSuit(myCard otherCard)
    {
        return this.getSuit() == otherCard.getSuit();
    }
}
