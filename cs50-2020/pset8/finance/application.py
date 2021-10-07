import os
from datetime import datetime
from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
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

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

@app.route("/")
@login_required
def index():
    total = 0
    act_price ={}
    act_totals = {}
    rows = db.execute("""SELECT SUM(shares), symbol, name, quote_price, total_price FROM buys WHERE user_id = ?
    GROUP BY symbol HAVING SUM(shares) > 0""", session["user_id"])
    for row in rows:
        symb = row["symbol"]
        act_price[symb] = lookup(symb)["price"]
        act_totals[symb] = (act_price[symb] * row["SUM(shares)"])

        total += act_totals[symb]

        act_totals[symb] = usd(act_totals[symb])
        act_price[symb] = usd(act_price[symb])

    money = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

    return render_template("index.html", rows=rows, r=act_price, t=act_totals, cash=usd(money[0]["cash"]), total=usd(total+money[0]["cash"]))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template("buy.html")
    else:
        if not request.form.get("symbol"):
            return apology("must submit a symbol")

        if not request.form.get("shares"):
            return apology("must submit a share")

        if (lookup(request.form.get("symbol")) == None):
            return apology("Not Found", 403)

        price = lookup(request.form.get("symbol"))["price"]
        shares = int(request.form.get("shares"))
        check_trade = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = check_trade[0]["cash"]
        if(cash < (price*shares)):
            return apology("Not money enough")

        n = db.execute("""INSERT INTO buys (user_id, symbol, name, shares, quote_price, total_price, time, type) VALUES
        (:user_id, :symbol, :name, :shares, :quote_price, :total_price, :time, :type)""",
        user_id = session["user_id"], symbol=request.form.get("symbol").upper(), name=lookup(request.form.get("symbol"))["name"],
        shares=shares, quote_price=usd(price), total_price= (-(shares*price)), time=datetime.now(), type = 0)

        cash = cash-(shares*price)
        updt = db.execute("UPDATE users SET cash = ? WHERE id = ?", cash, session["user_id"])
        return redirect("/")

@app.route("/add", methods=["GET", "POST"] )
@login_required
def add():
    if request.method == "GET":
        return render_template("add.html")

    if not request.form.get("cash"):
        return apology("must sumbit an amount of cash")
    money = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

    total = float(request.form.get("cash")) + money[0]["cash"]
    updt = db.execute("UPDATE users SET cash = ? WHERE id = ?", total, session["user_id"])
    return redirect("/")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    hist = db.execute("SELECT symbol, shares, quote_price, time FROM buys WHERE user_id = ?", session["user_id"])
    return render_template("history.html", rows=hist)

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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

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
    if request.method == "GET":
        return render_template("quote.html")
    else:
        if not request.form.get("symbol"):
            return apology("must submit a symbol", 400)
        if (lookup(request.form.get("symbol")) == None):
            return apology("Not Found", 403)

        quote = usd(lookup(request.form.get("symbol"))["price"])
        name = lookup(request.form.get("symbol"))["name"]
        symbol = lookup(request.form.get("symbol"))["symbol"]
        return render_template("quoted.html", quote = quote, name = name, symbol = symbol)

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        username = request.form.get("username")
        #ensure username was submited
        if not username:
            return apology("must provide an username", 403)

        #ensure password was submited
        password = request.form.get("password")
        if not password:
            return apology("must provide a password", 403)

        #ensure confirmation was submited
        confirmation = request.form.get("confirmation")
        if not confirmation:
            return apology("must provide a confirmation", 403)

        #ensure the password and the confirmation are the same
        if (password != confirmation):
            return apology("passwords dont match", 403)

        user_check = db.execute("SELECT COUNT(*) FROM users WHERE username=:name", name=username)
        if(user_check[0]['COUNT(*)'] > 0):
            return apology("Username already registred")

        db.execute("INSERT INTO users (username, hash) VALUES (:username, :password)",
        username = username, password = generate_password_hash(password))
        return redirect("/login")
priceprice
@app.route("/new_pass", methods=["GET", "POST"])
def change():
    """New Password"""
    if request.method == "GET":
        return render_template("new_pass.html")
    else:
        if not request.form.get("username"):
            return apology("must submit a username", 400)

        old_password=request.form.get("old_password")
        if not request.form.get("old_password"):
            return apology("must submit a old password", 400)

        if not request.form.get("new_password"):
            return apology("must submit a new password", 400)

        if not request.form.get("confirmation"):
            return apology("must submit a confirmation", 400)

        if (request.form.get("confirmation") != request.form.get("new_password")):
            return apology("new password and confirmation must be the same")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))
        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("old_password")):
            return apology("invalid username and/or password", 403)

        db.execute("UPDATE users SET hash = :password WHERE username = :user",
        password=generate_password_hash(request.form.get("new_password")), user=request.form.get("username"))
        return redirect("/login")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        symbols = db.execute("SELECT symbol FROM buys WHERE user_id = ? AND type = 0 GROUP BY symbol", session["user_id"])
        return render_template("sell.html", symbols=symbols)
    else:
        if not request.form.get("symbol"):
            return apology("must submit a symbol")

        if not request.form.get("shares"):
            return apology("must submit a share")

        if (lookup(request.form.get("symbol")) == None):
            return apology("Not Found", 403)

        max_shares = db.execute("SELECT SUM(shares) FROM buys WHERE user_id = ? AND symbol = ? GROUP BY symbol", session["user_id"], request.form.get("symbol"))

        if max_shares[0]["SUM(shares)"] < int(request.form.get("shares")):
            return apology("exceeded the maximum number of shares")

        price = lookup(request.form.get("symbol"))["price"]

        shares = int(request.form.get("shares"))

        n = db.execute("""INSERT INTO buys (user_id, symbol, name, shares, quote_price, total_price, time, type) VALUES
        (:user_id, :symbol, :name, :shares, :quote_price, :total_price, :time, :type)""",
        user_id = session["user_id"], symbol=request.form.get("symbol").upper(), name=lookup(request.form.get("symbol"))["name"],
        shares= (-shares), quote_price=usd(price), total_price= (shares*price), time=datetime.now(), type = 1)

        money = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

        total = money[0]["cash"] + (shares*price)

        updt = db.execute("UPDATE users SET cash = ? WHERE id = ?", total, session["user_id"])
        return redirect("/")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)