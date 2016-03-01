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

import models.Card;
import models.Game;
import ninja.Context;
import ninja.Result;
import ninja.Results;
import models.AmericanGame;
import models.SpanishGame;

import com.google.inject.Singleton;
import ninja.params.PathParam;


@Singleton
public class ApplicationController {

    public Result index() {
        return Results.html();
    }

    public Result Choice() {
        return Results.html().template("views/AcesUp/Choice.html");
    }

    public Result SpanishAcesUp() {
        return Results.html().template("views/AcesUp/SpanishAcesUp.flt.html");
    }

    public Result AcesUp() {
        return Results.html().template("views/AcesUp/AcesUp.flt.html");
    }
    public Result gameGet(){
        Game g = new AmericanGame();
        g.shuffle();
        g.dealFour();

        return Results.json().render(g);
    }
    public Result SpanishgameGet(){
        Game g = new SpanishGame();
        g.shuffle();
        g.dealFour();

        return Results.json().render(g);
    }

    public Result SdealPost(Context context, SpanishGame g) {
        if(context.getRequestPath().contains("deal")){
            g.dealFour();
        }
        return Results.json().render(g);
    }

    public Result dealPost(Context context, AmericanGame g) {
        if(context.getRequestPath().contains("deal")){
            g.dealFour();
        }
        return Results.json().render(g);
    }

    public Result removeCard(Context context, @PathParam("column") int colNumber, AmericanGame g){
        g.remove(colNumber);
        return  Results.json().render(g);
    }

    public Result SremoveCard(Context context, @PathParam("column") int colNumber, SpanishGame g){
        g.remove(colNumber);
        return  Results.json().render(g);
    }

    public Result moveCard(Context context, @PathParam("columnFrom") int colFrom, @PathParam("columnTo") int colTo, AmericanGame g) {
        g.move(colFrom, colTo);
        return Results.json().render(g);
    }

    public Result SmoveCard(Context context, @PathParam("columnFrom") int colFrom, @PathParam("columnTo") int colTo, SpanishGame g) {
        g.move(colFrom, colTo);
        return Results.json().render(g);
    }
}
