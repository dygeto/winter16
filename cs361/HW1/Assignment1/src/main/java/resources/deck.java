package resources;

/**
 * Created by erinsullens on 1/19/16.
 */
public class deck {

    
    public myCard[] cards;
    int rank = 1;

   public deck(){
        cards = new myCard[52];

        for(int i = 0; i<13; i++){
            cards[i] = new myCard(rank,'c');
            rank ++;
        }
        rank = 1;
        for(int i = 13; i<26; i++){
            cards[i] = new myCard(rank,'s');
            rank ++;
        }
        rank = 1;
        for(int i = 26; i<39; i++){
            cards[i] = new myCard(rank,'h');
            rank ++;
        }
        rank = 1;
        for(int i = 39; i<52; i++){
            cards[i] = new myCard(rank,'d');
            rank ++;
        }

    }
    public void shuffle(){
        int rand;
        myCard temp;
        for(int i = 0; i <52; i++){

            rand = (int)Math.random()*51;
            temp = cards[rand];
            cards[rand] = cards[i];
            cards[i] = temp;

        }
    }
    public myCard[] deal(){
        myCard[] dealtCards = new myCard[4];
        int fourCards = 0;
        for(int i = 0; i<52; i++){
            if(!this.cards[i].hasBeenPlayed){
                dealtCards[fourCards] = this.cards[i];
                fourCards++;
            }
            if(fourCards==4){
                break;
            }
        }

        return dealtCards;
    }

}
