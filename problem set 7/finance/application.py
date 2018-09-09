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
    symbol_shares = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE username = :username GROUP BY symbol", username=session["username"])

    # Determine the holdings for each symbol and the total holdings
    total_holdings = 0
    for company in symbol_shares:
        # Find out total worth of the symbol
        symbol = company['symbol']              # get the current symbol
        name_price_symbol = lookup(symbol)      # look up the name and price
        price = name_price_symbol['price']      # record the price
        company['price'] = price                # associate the price with the current symbol

        # Calculate holding = shares * value / shares
        holding = int(company['SUM(shares)']) * price   # calculate total current worth of shares for that symbol
        company['holding'] = holding                    # associate the total current worth  with the current symbol

        total_holdings = total_holdings + holding         # sum up all the holdings for all the symbols


    # Determine cash and overall total
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
    symbol_shares.append(cash[0])

    total = total_holdings + cash[0]['cash']
    symbol_shares.append({'total': total})

    # Send the symbols, shares, price, and totals to the home page
    return render_template("index.html", chart=symbol_shares)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        name_price_symbol = lookup(request.form.get("symbol"))

        # Ensure password was submitted
        if name_price_symbol == None:
            return apology("could not find symbol", 400)

        # Ensure username was submitted
        if request.form.get("shares").isdigit() == False:
            return apology("must input a whole number of shares", 400)
        elif int(request.form.get("shares")) <= 0:
            return apology("must input positive number of shares", 400)
        elif request.form.get("shares").isnumeric == False:
            return apology("must input a number of shares", 400)


        shares = int(request.form.get("shares"))

        name = name_price_symbol["name"]
        price = float(name_price_symbol["price"])
        symbol = request.form.get("symbol")

        cost = shares * price

        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        cash = cash[0]['cash']

        cash = cash - cost

        if cash < 0:
           return apology("nigga u poor", 403)

        db.execute('INSERT INTO "transactions" ("username","symbol","price", "shares") VALUES (:username,:symbol,:price, :shares)', username=session["username"], symbol=symbol, price=price, shares=shares)

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

    rows = db.execute("SELECT * FROM transactions WHERE username = :username ORDER BY time", username=session["username"])

    return render_template("index.html", chart=rows)


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

        # Ensure username was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        name_price_symbol = lookup(request.form.get("symbol"))

        # Ensure password was submitted
        if name_price_symbol == None:
            return apology("could not find symbol", 400)

        name = name_price_symbol["name"]
        price = name_price_symbol["price"]
        symbol = name_price_symbol["symbol"]

        # Redirect user to home page
        return render_template("quoted.html", name_price_symbol=name_price_symbol, price=price)

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

         # Query database for username
        usernames = db.execute("SELECT username FROM users")

        found = 0
        for user in usernames:
            if request.form.get("username") == user['username']:
                found = 1
        if found == 1:
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

        # Insert new row into database
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

        # Ensure username was submitted
        #if not request.form("symbol"):
            #return apology("must provide symbol", 403)

        symbol = request.form["symbol"]

        name_price_symbol = lookup(symbol)

        # Ensure password was submitted
        if name_price_symbol == None:
            return apology("could not find symbol online", 403)

        symbol_shares = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE username = :username GROUP BY symbol", username=session["username"])

        found = 0
        for company in symbol_shares:
            if symbol == company['symbol']:
                found = 1
                sumShares = company['SUM(shares)']
        if found == 0:
            return apology("could not find symbol in your portfolio", 403)

        # Ensure username was submitted
        if int(request.form.get("shares")) <= 0:
            return apology("must input positive number of shares", 400)

        if int(request.form.get("shares")) > sumShares:
            return apology("you do not have enough shares to sell", 400)

        shares = 0 - int(request.form.get("shares"))

        price = float(name_price_symbol["price"])

        benefit = -shares * price

        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        cash = cash[0]['cash']

        cash = cash + benefit

        db.execute('INSERT INTO "transactions" ("username","symbol","price", "shares") VALUES (:username,:symbol,:price, :shares)', username=session["username"], symbol=symbol, price=price, shares=shares)

        db.execute('UPDATE "users" SET "cash"= :cash WHERE "rowid" = :id', cash=cash, id=session["user_id"])


        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        symbols = db.execute("SELECT symbol FROM transactions WHERE username= :username GROUP BY symbol", username=session["username"])

        symbol_list = []
        for company in symbols:

            symbol_list.append(company["symbol"])

        return render_template("sell.html", symbols=symbol_list)

def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
