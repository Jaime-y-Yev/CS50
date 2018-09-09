import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get list of symbols their total shares
    symbol_shares = db.execute("SELECT symbol, SUM(shares) AS shares FROM transactions WHERE username = :username GROUP BY symbol", username=session["username"])

    # Determine the holdings for each symbol and the total holdings
    total_holdings = 0
    for company in symbol_shares:
        # Find out name and total worth of the symbol
        symbol = company['symbol']              # get the current symbol
        name_price_symbol = lookup(symbol)      # look up the name and price
        name = name_price_symbol['name']        # record the name
        price = name_price_symbol['price']      # record the price
        company['name'] = name                  # store name in symbol_shares next to its symbol
        company['price'] = price                # store price in symbol_shares next to its symbol

        # Calculate holding = shares * value / shares
        holding = int(company['shares']) * price   # calculate total current worth of shares for that symbol
        # holding = int(company['SUM(shares)']) * price   # calculate total current worth of shares for that symbol
        company['holding'] = holding                    # associate the total current worth  with the current symbol

        total_holdings = total_holdings + holding         # sum up all the holdings for all the symbols

    # Determine cash and overall total
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
    symbol_shares.append(cash[0])

    total = total_holdings + cash[0]['cash']
    symbol_shares.append({'total': total})


    # Send the symbols, shares, price, and totals to the home page
    return render_template("index.html", table=symbol_shares)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        # Ensure symbol exists
        symbol = request.form.get("symbol")
        name_price_symbol = lookup(symbol)
        if name_price_symbol == None:
            return apology("could not find symbol", 400)

        # Ensure number of shares was submitted
        if not request.form.get("shares"):
            return apology("must provide number of shares", 400)

        shares = request.form.get("shares")

        # Ensure number of shares is purely a whole positive number
        if shares.isdigit() == False:
            return apology("must input a whole number of shares", 400)
        elif int(shares) <= 0:
            return apology("must input positive number of shares", 400)
        elif shares.isnumeric == False:
            return apology("must input a number of shares", 400)

        shares = int(request.form.get("shares"))

        # Calculate the cost for this symbol and number of shares
        price = float(name_price_symbol["price"])
        cost = shares * price

        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        cash = cash[0]['cash']

        cash = cash - cost

        # Make sure user has enough cash for this purchase
        if cash < 0:
           return apology("nigga u poor", 403)


        # Add this transaction to the history
        db.execute('INSERT INTO "transactions" ("username","symbol","price", "shares") VALUES (:username,:symbol,:price, :shares)', username=session["username"], symbol=symbol, price=price, shares=shares)

        # Update the users cash amount
        db.execute('UPDATE "users" SET "cash"= :cash WHERE "rowid" = :id', cash=cash, id=session["user_id"])


        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute("SELECT * FROM transactions WHERE username = :username ORDER BY time", username=session["username"])

    return render_template("history.html", table=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["username"] = rows[0]["username"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        name_price_symbol = lookup(request.form.get("symbol"))

        # Ensure symbol was found
        if name_price_symbol == None:
            return apology("could not find symbol", 400)

        # Extract name, price, and symbol
        name = name_price_symbol["name"]
        price = name_price_symbol["price"]
        symbol = name_price_symbol["symbol"]

        # Display name, price, and symbol
        return render_template("quoted.html", name=name, price=price, symbol=symbol)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        desired_username = request.form.get("username")

        # Check that desired_username isn't already registered
        usernames = db.execute("SELECT username FROM users WHERE username=:desired_username", desired_username=desired_username)    # query database for desired username

        if len(usernames) != 0:                                 # if something came up under that username, it means it already exists
            return apology("username already exists", 400)


        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password was confirmed
        if not request.form.get("confirmation"):
            return apology("must confirm password", 400)

        # Ensure password and confirmation match
        elif  request.form.get("confirmation") != request.form.get("password"):
            return apology("password and confirmation must match", 400)

        hash_password = generate_password_hash(request.form.get("password"))


        # Insert new user into users
        db.execute('INSERT INTO users ("id","username","hash") VALUES (NULL,:username,:password)', username=request.form.get("username"), password=hash_password)


        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        symbol = request.form["symbol"]

        # Ensure a symbol was submitted
        if symbol == "Choose a symbol":
            return apology("choose a symbol", 403)

        name_price_symbol = lookup(symbol)

        # Ensure the symbol exists
        if name_price_symbol == None:
            return apology("could not find symbol online", 403)

        symbol_shares = db.execute("SELECT symbol, SUM(shares) AS shares FROM transactions WHERE username = :username GROUP BY symbol", username=session["username"])

        # Find the total shares owned for this symbol
        sumShares = next(company['shares'] for company in symbol_shares if company["symbol"] == symbol)

        # Record number of shares to sell
        shares = int(request.form.get("shares"))

        # Ensure a positive number shares to sell was submitted
        if shares <= 0:
            return apology("must input positive number of shares", 400)
        # Ensure user has enough shares to sell
        if shares > sumShares:
            return apology("you do not have enough shares to sell", 400)

        # Calculate the benefit gained from selling the shares and new cash amount
        price = float(name_price_symbol["price"])       # find out the current price per share
        benefit = shares * price                        # calculate the total benefit of this sale

        currentCash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])        # find out how much cash the user currently has
        currentCash = currentCash[0]['cash']

        cash = currentCash + benefit            # calculate how much cash the user will now have

        db.execute('UPDATE "users" SET "cash"= :cash WHERE "rowid" = :id', cash=cash, id=session["user_id"])    # update the new cash amount for this user


        # Update the transaction history. NOTE NEGATIVE SIGN IN shares=-shares TO INDICATE A SALE IN THE TRANSACTION HISTORY
        db.execute('INSERT INTO "transactions" ("username","symbol","price", "shares") VALUES (:username,:symbol,:price, :shares)', username=session["username"], symbol=symbol, price=price, shares=-shares)


        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Determine symbols the user owns, so they can populate a drop-down menu of symbols to sell
        symbols = db.execute("SELECT symbol FROM transactions WHERE username= :username GROUP BY symbol", username=session["username"])

        return render_template("sell.html", symbols=symbols)


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
