package models;

public class gameLogic {
    private int winCount;
    private int score;
    myStack[] cardStack; //cardStack will be the four spots that cards are dealt to, not yet made

    gameLogic() {
        winCount = 0;
        score = 0;
        cardStack = new myStack[3];
    }

    //Function to be called if no cards are left in the deck
    public void winCheck() {
        int aceCount = 0;
        for (int i = 0; i < 4; i++) {
            if (cardStack[i].getRank() == 'A' && cardStack[i].getSize() == 1) {
                aceCount++;
            }
        }
        if (aceCount == 4) {
            System.out.println("You win!");
            winCount++;
        }
    }
}
