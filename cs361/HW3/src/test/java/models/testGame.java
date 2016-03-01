package models;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by michaelhilton on 1/25/16.
 */
public class testGame {

    @Test
    public void testAmericanGameCreation() {
        Game g = new AmericanGame();
        assertNotNull(g);
    }

    @Test
    public void testSpanishGameCreation() {
        Game s = new SpanishGame();
        assertNotNull(s);
    }


    @Test
    public void testAmericanDeckCount(){
        AmericanGame g = new AmericanGame();
        assertEquals(52,g.deck.size());
        g.remove(2);
        assertEquals(51,g.deck.size());
    }

    @Test
    public void testSpanishDeckCount() {
        Game s = new SpanishGame();
           assertEquals(40,s.deck.size());
        s.remove(2);
           assertEquals(39,s.deck.size());
    }


    @Test
    public void testGameBuildDeck(){
        AmericanGame g = new AmericanGame();
        assertEquals(52,g.deck.size());
    }

    @Test
    public void testGameBuildSpanishDeck(){
        Game g = new SpanishGame();
       assertEquals(40,g.deck.size());
    }

    @Test
    public void testGameInit(){
        AmericanGame g = new AmericanGame();
        g.buildDeck();
        g.shuffle();
        assertNotEquals(2,g.deck.get(0).getValue());
    }

    @Test
    public void testSpanishGameInit(){
        Game g = new SpanishGame();
        g.buildDeck();
        g.shuffle();
        assertNotEquals(0,g.deck.get(0).getValue());
    }

    @Test
    public void testGameStart(){
        AmericanGame g = new AmericanGame();
        g.buildDeck();
        g.shuffle();
        g.dealFour();
        assertEquals(1,g.cols.get(0).size());
        assertEquals(1,g.cols.get(1).size());
        assertEquals(1,g.cols.get(2).size());
        assertEquals(1,g.cols.get(3).size());
    }

    @Test
    public void testSpanishGameStart(){
        Game g = new SpanishGame();
        g.buildDeck();
        g.shuffle();
        g.dealFour();
        assertEquals(1,g.cols.get(0).size());
        assertEquals(1,g.cols.get(1).size());
        assertEquals(1,g.cols.get(2).size());
        assertEquals(1,g.cols.get(3).size());
    }

    @Test
    public void testCustomDeal(){
        AmericanGame g = new AmericanGame();
        g.buildDeck();
        g.customDeal(0,3,6,9);
        assertEquals("2Clubs",g.cols.get(0).get(0).toString());
        assertEquals("3Clubs",g.cols.get(1).get(0).toString());
        assertEquals("4Clubs",g.cols.get(2).get(0).toString());
        assertEquals("5Clubs",g.cols.get(3).get(0).toString());
    }

    @Test
    public void testSpanishCustomDeal(){
        Game g = new SpanishGame();
        g.buildDeck();
        g.customDeal(0,3,6,9);
        assertEquals("1Clubs",g.cols.get(0).get(0).toString());
        assertEquals("2Clubs",g.cols.get(1).get(0).toString());
        assertEquals("3Clubs",g.cols.get(2).get(0).toString());
        assertEquals("4Clubs",g.cols.get(3).get(0).toString());
    }

    @Test
    public void testRemoveFunction(){
        AmericanGame g = new AmericanGame();
        g.buildDeck();
        g.customDeal(0,3,6,9);
        g.remove(2);
        assertEquals(0,g.cols.get(2).size());
    }

    @Test
    public void testSpanishRemoveFunction(){
        Game g = new SpanishGame();
        g.buildDeck();
        g.customDeal(0,3,6,9);
        g.remove(2);
        assertEquals(0,g.cols.get(2).size());
    }



}
