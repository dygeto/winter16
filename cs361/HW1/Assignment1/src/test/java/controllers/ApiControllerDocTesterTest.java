/**
 * Copyright (C) 2013 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package controllers;
import resources.myCard;
import resources.deck;
import ninja.NinjaDocTester;
import org.doctester.testbrowser.Request;
import org.doctester.testbrowser.Response;
import org.junit.Test;
import static org.hamcrest.CoreMatchers.containsString;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertThat;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.*;
public class ApiControllerDocTesterTest extends NinjaDocTester {

    String URL_INDEX = "/";
    String URL_ACES_UP = "/AcesUp";

    @Test
    public void testGetIndex() {

        Response response = makeRequest(
                Request.GET().url(
                        testServerUrl().path(URL_INDEX)));

        assertThat(response.payload, containsString("Hello World!"));
        assertThat(response.payload, containsString("BAM!"));
    }

    @Test
    public void testAcesUp() {
        Response response = makeRequest(
                Request.GET().url(
                        testServerUrl().path(URL_ACES_UP)));

        assertThat(response.payload, containsString("Aces Up"));
        assertThat(response.payload, containsString("columnOfCards"));
    }
    @Test
    public void testCardCreation(){
        myCard Cardy;
        Cardy = new myCard();
        assert(Cardy.getRank() == ' ');
    }
    @Test
    public void testCardRankSettingGetting(){
        myCard Cardy;
        Cardy = new myCard();
        Cardy.setRank(3);
        assert(Cardy.getRank() == 3);
    }
    @Test
    public void testCardSuitSettingGetting(){
        myCard Cardy;
        Cardy = new myCard();
        Cardy.setSuit('H');
        assert(Cardy.getSuit() == 'H');
    }
    public void testCardConstructor(){
        myCard Cardy;
        Cardy = new myCard(3, 'H');
        assert(Cardy.getSuit() == 'H');
        assert(Cardy.getRank() == 3);
    }

    @Test
    public void testCardComparison(){
        myCard curCard;
        curCard = new myCard();
        myCard otherCard;
        otherCard = new myCard();

        //Tests for comparison function:
        //Returns 2 if curCard > otherCard, 0 if curCard = otherCard, 1 if curCard < otherCard, 3 if comparison failed
        for(int i=13; i<26; i++) {
            curCard.setRank(i);
            assert (curCard.getRank() == i);

            //Test if current card rank is greater than other card rank
            otherCard.setRank(i-1);
            assert(otherCard.getRank() == i-1);
            curCard.compareRank(otherCard);
            assert(curCard.compareRank(otherCard) != 3); //something wrong with compareRank() function
            assert(curCard.compareRank(otherCard) == 2);


            //Test if current card rank equal to the other card rank
            otherCard.setRank(i);
            assert (otherCard.getRank() == i);
            curCard.compareRank(otherCard);
            assert(curCard.compareRank(otherCard) != 3); //something wrong with compareRank() function
            assert (curCard.compareRank(otherCard) == 0);

            //Test if current card rank is less than the other card rank
            otherCard.setRank(i+1);
            assert (otherCard.getRank() == i+1);
            curCard.compareRank(otherCard);
            assert(curCard.compareRank(otherCard) != 3); //something wrong with compareRank() function
            assert (curCard.compareRank(otherCard) == 1);

        }


        //Test if current card suit does or doesn't match other card suit
        char[] suitsArr = {'S', 'H', 'D', 'C'};
        for(int i=0; i<4; i++) {
            curCard.setSuit(suitsArr[i]);
            assert (curCard.getSuit() == suitsArr[i]);
            otherCard.setSuit(suitsArr[i]);
            assert (otherCard.getSuit() == suitsArr[i]);
            curCard.compareSuit(otherCard);
            assertTrue(curCard.compareSuit(otherCard));

            if (i == 0) { //Put if statement, so it doesn't go over the suitsArr limit
                curCard.setSuit(suitsArr[i]);
                assert (curCard.getSuit() == suitsArr[i]);
                otherCard.setSuit(suitsArr[i+1]);
                assert (otherCard.getSuit() == suitsArr[i+1]);
                curCard.compareSuit(otherCard);
                assertFalse(curCard.compareSuit(otherCard));
            } else {
                curCard.setSuit(suitsArr[i]);
                assert (curCard.getSuit() == suitsArr[i]);
                otherCard.setSuit(suitsArr[i-1]);
                assert (otherCard.getSuit() == suitsArr[i-1]);
                curCard.compareSuit(otherCard);
                assertFalse(curCard.compareSuit(otherCard));
            }
        }

    }

    @Test
    public void testDeck(){
        deck myDeck = new deck();
        myDeck.cards[0] = new myCard(2,'c');
    }

    @Test
    public void testCardInDeckClubs(){
        deck myDeck = new deck();
        assertEquals(myDeck.cards[0].getRank(),1);
        assertEquals(myDeck.cards[0].getSuit(),'c');
        assertEquals(myDeck.cards[1].getRank(),2);
        assertEquals(myDeck.cards[0].getSuit(),'c');
        for(int i = 0; i<13; i++){
            assertEquals(myDeck.cards[i].getRank(),i+1);
            assertEquals(myDeck.cards[i].getSuit(),'c');
        }
    }

    @Test
    public void testCardAndRankSpades(){
        deck myDeck = new deck();
        int myRank = 1;
        for(int i = 13; i<26; i++){
            assertEquals(myDeck.cards[i].getRank(),myRank);
            assertEquals(myDeck.cards[i].getSuit(),'s');
            myRank ++;
        }
    }

    @Test
    public void testCardAndRankHearts(){
        deck myDeck = new deck();
        int myRank = 1;
        for(int i = 26; i<39; i++){
            assertEquals(myDeck.cards[i].getRank(),myRank);
            assertEquals(myDeck.cards[i].getSuit(),'h');
            myRank ++;
        }
    }

    @Test
    public void testCardAndRankDiamonds(){
        deck myDeck = new deck();
        int myRank = 1;
        for(int i = 39; i<52; i++){
            assertEquals(myDeck.cards[i].getRank(),myRank);
            assertEquals(myDeck.cards[i].getSuit(),'d');
            myRank ++;
        }
    }

    @Test
    public void testHasBeenPlayed(){
        deck myDeck = new deck();
        assertEquals(myDeck.cards[0].hasBeenPlayed, false);
    }
    @Test
    public void testDeal(){
        deck myDeck = new deck();
        myCard[] dealtCards;
        dealtCards = new myCard[4];
        dealtCards = myDeck.deal();
        assertNotEquals(dealtCards[0],null );
    }
}

