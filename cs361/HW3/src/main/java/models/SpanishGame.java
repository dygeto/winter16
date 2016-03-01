package models;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

/**
 * Created by michaelhilton on 1/25/16.
 */
public class SpanishGame extends Game {



    public void buildDeck() {
        for(int i = 1; i < 11; i++){
            deck.add(new Card(i,Suit.Coins));
            deck.add(new Card(i,Suit.Cups));
            deck.add(new Card(i,Suit.Swords));
            deck.add(new Card(i,Suit.Clubs));
        }
    }



}
