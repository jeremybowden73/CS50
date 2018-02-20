from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp
from datetime import datetime

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():

    # clear the index table
    db.execute("DELETE FROM 'index'")

    # check if the user owns any shares. If not, the 'index' page will have nothing to display
    current_user = db.execute("SELECT qty_shares FROM 'users' WHERE id = :idd", idd=session["user_id"])
    if current_user[0]['qty_shares'] == 0:
        return apology("You don't own any shares! Wanna buy some? Click Quote or Buy!")

    # copy the symbol, name, and total quantity of shares data from 'history' table into 'index' table for the current user
    db.execute("INSERT INTO 'index' (symbol, name, quantity) SELECT symbol, name, sum(qty) FROM 'history' WHERE user_id =:idd GROUP BY symbol", idd=session["user_id"])

    # create a list of dicts for the stock symbol for every row in the 'index' table
    symbollist = db.execute("SELECT symbol FROM 'index'")
    # iterate over the dicts, each time get the current share price and add it to 'index'
    for i in range(len(symbollist)):
        for x in symbollist[i]:
            # use the lookup function (in helpers.py) to generate data for each stock the user owns (as a dict)
            quote = lookup((symbollist[i][x]))
            # populate the currentprice field with the data from 'quote' for the current stock
            db.execute("UPDATE 'index' SET currentprice = :currentprice WHERE symbol = :symbol", currentprice=quote['price'], symbol=(symbollist[i][x]))
            # populate the totalvalue field for the current stock
            db.execute("UPDATE 'index' SET totalvalue = quantity*currentprice WHERE symbol = :symbol", symbol=(symbollist[i][x]))



    # create stockslist, a LIST of DICT objects, from the 'index' table. Each dict contains the (key, value) pairs
    # for the fields and cells for each record in the index table (i.e. a dict for each stock the user owns).
    stockslist = db.execute("SELECT symbol, name, quantity, currentprice, totalvalue FROM 'index'")

    # to calculate the total value of all of all the shares the user owns, need to add together all the
    # 'totalvalue's from the 'index' table by iterating over the dicts, adding the value of the key 'totalvalue' each time.
    # t is the sum total of the shares (it is a string!).
    t = 0
    for j in range(len(stockslist)):
        t += stockslist[j]['totalvalue']

    # also need to display in "index.html" the current user's cash balance (c is still a string)
    users_cash = db.execute("SELECT cash FROM users WHERE id = :idd", idd=session["user_id"])
    c = (users_cash[0]["cash"])

    # calculate the grand total of the users shares + cash
    g = "{0:.2f}".format(float(c)+float(t))   # cast to floats to avoid adding strings

    # for the index table we need to show the 'currentprice' and 'totalvalue' to 2 decimal places to
    # resemble currency, so iterate over stockslist to do this
    for i in range(len(stockslist)):
        stockslist[i]['currentprice'] = "{0:.2f}".format(stockslist[i]['currentprice'])
        stockslist[i]['totalvalue'] = "{0:.2f}".format(stockslist[i]['totalvalue'])

    # now we can pass the stocks list, the cash balance, and the grand total to index.html"
    return render_template("index.html", stocks=stockslist, cash="{0:.2f}".format(c), grandtotal=g)



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # use the lookup function (in helpers.py) to generate data for the stock (as a dict)
        quote = lookup(request.form.get("stockname"))

        # check the quantity entered is a numeric value and create int of 'quantity of shares to buy'
        if not request.form.get("quantity").isdigit():
            return apology("Please enter only digits for the quantity of shares.")

        # create int of 'quantity of shares to buy'
        quantity = int(request.form.get("quantity"))


        if quote == None:
            return apology("Invalid stock code")

        elif quantity <= 0:
            return apology("Number of shares must be 1 or more")

        # if the sale is OK, calculate the value of the sale
        trans_value = (quantity * quote['price'])

        # how much cash does the currently signed in user have?
        cash_available = db.execute("SELECT cash FROM users WHERE id = :idd", idd=session["user_id"])
        # check the user has enough cash to buy the shares
        if (cash_available[0]["cash"]) <= trans_value:
            return apology("Not enough cash to buy that many shares!")

        # if it's all OK, add the transaction to the 'history' db as a "Bought" transaction (one long line!)
        db.execute("INSERT INTO history (symbol, name, buysell, price, qty, value, date, user_id) \
        VALUES(:symbol, :name, :buysell, :price, :qty, :value, :date, :user_id)",
        symbol=quote['symbol'], name=quote['name'], buysell='Bought', price=quote['price'], qty=quantity,
        value=trans_value, date=datetime.now().strftime('%Y-%m-%d %H:%M:%S'), user_id=session["user_id"])

        # update current users cash by subtractinging the transaction value
        db.execute("UPDATE users SET cash = (cash - :cost) WHERE id = :idd", cost=trans_value, idd=session["user_id"])
        # update current users total number of shares by adding the quantity just bought
        db.execute("UPDATE users SET qty_shares = (qty_shares + :quant) WHERE id = :idd", quant=quantity, idd=session["user_id"])

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    return render_template("buy.html")


@app.route("/history", methods=["GET", "POST"])
@login_required
def history():

    # check if the user has owned any shares. If not, the 'history' page will have nothing to display
    current_user = db.execute("SELECT qty_shares FROM 'users' WHERE id = :idd", idd=session["user_id"])
    if current_user[0]['qty_shares'] == 0:
        return apology("You don't own any shares! Wanna buy some? Click Quote or Buy!")

    # create history, a LIST of DICT objects, from the 'history' table, for the current user.
    history = db.execute("SELECT symbol, name, buysell, price, qty, value, date FROM 'history' WHERE user_id = :idd", idd=session["user_id"])

    # iterate over the dicts, changing the value of currentprice and totalprice to be 2 decimal places (so it resembles currency)
    for j in range(len(history)):
        history[j]['price'] = "{0:.2f}".format(history[j]['price'])
        history[j]['value'] = "{0:.2f}".format(history[j]['value'])

    return render_template("history.html", historydata=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in by their unique id, and by their username which they created
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # use the lookup function (in helpers.py) to generate data for the stock (as a dict)
        quote = lookup(request.form.get("stocksymbol"))
        if quote == None:
            return apology("Invalid stock code")

        # if it's a real stock, render the html page and display the data provided by lookup
        return render_template("quote_display.html", name=quote['name'], price=quote['price'], symbol=quote['symbol'])

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote_request.html")



@app.route("/register", methods=["GET", "POST"])
def register():

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        if request.form.get("username") == "" or request.form.get("password") == "" or request.form.get("confirmpassword") == "":
            return apology("To register please enter a username and a password (twice!)")
        elif request.form.get("password") != request.form.get("confirmpassword"):
            return apology("Your passwords did not match!")

        # create a hashed password from the users password
        hashpw = pwd_context.hash(request.form.get("password"))

        # add the username and hashed password to the 'users' table. The username field in 'users' is Unique, so if the username
        # is already in the db the db.execute function will return None (to 'result'), otherwise it returns the value of
        # the newly inserted row's primary key (id field).
        result = db.execute("INSERT INTO users (username, hash) VALUES(:name, :hashed)", name=request.form.get("username"), hashed=hashpw)

        if result == None:
            return apology("Username already exists!")

        # if registration is OK, remember them by storing the new row's primary key to "user_id", then redirect user to home page
        session["user_id"] = result
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # use the lookup function (in helpers.py) to generate data for the stock (as a dict)
        quote = lookup(request.form.get("stockname"))

        # check the quantity entered is a numeric value and create int of 'quantity of shares to sell'
        if not request.form.get("quantity").isdigit():
            return apology("Please enter only digits for the quantity of shares.")

        quantity = int(request.form.get("quantity"))

        if quote == None:
            return apology("Invalid stock code")

        elif quantity <= 0:
            return apology("Number of shares to sell must be 1 or more")

        # does the user have that many shares of the stock to sell?
        shares = db.execute("SELECT SUM(qty) FROM history WHERE symbol = :sym AND user_id = :idd", sym=quote['symbol'], idd=session["user_id"])
        if (shares[0]["SUM(qty)"]) == None:
            return apology("You don't have any of those shares to sell!")
        if (shares[0]["SUM(qty)"]) < quantity:
            return apology("You don't have that many shares to sell!")

        # if the sale is OK, calculate the value of the sale, then
        # add it to the 'history' db as a "Sold" transaction (one long line!)
        trans_value = (quantity * quote['price'])

        db.execute("INSERT INTO history (symbol, name, buysell, price, qty, value, date, user_id) \
        VALUES(:symbol, :name, :buysell, :price, :qty, :value, :date, :user_id)",
        symbol=quote['symbol'], name=quote['name'], buysell='Sold', price=-(quote['price']), qty=-quantity,
        value=-trans_value, date=datetime.now().strftime('%Y-%m-%d %H:%M:%S'), user_id=session["user_id"])

        # update current users cash by adding the transaction value
        db.execute("UPDATE users SET cash = (cash + :cost) WHERE id = :idd", cost=trans_value, idd=session["user_id"])
        # update current users total number of shares by subtracting the quantity just bought
        db.execute("UPDATE users SET qty_shares = (qty_shares - :quant) WHERE id = :idd", quant=quantity, idd=session["user_id"])

        # redirect user to home page
        return redirect(url_for("index"))



    # else if user reached route via GET (as by clicking a link or via redirect)
    return render_template("sell.html")


