'''
Created on 11.03.2018

@author: Joshua Follner 
'''
import time
from selenium import webdriver
from random import randint

class TravianBot:
    def __init__(self, driver, start, end):
        self.driver = driver
        self.start = start
        self.end = end

        self.url1 = 'https://ts29.travian.de/login.php'
        self.url2 = 'https://ts29.travian.de/dorf2.php?newdid=8253&'
        self.urlRaid = "https://ts29.travian.de/build.php?tt=99&id=39"

    def getIntervall(self):
        self.start = input("Enter linke Intervallgrenze: ")
        self.end = input("Enter rechte Intervallgrenze: ")

    
    def login(self):
        self.driver.get(self.url1)
        time.sleep(2.5)
        user = self.driver.find_element_by_name("name")
        password = self.driver.find_element_by_name("password")
        time.sleep(1)
        user.clear()
        user.send_keys("Dicke Berta")
        time.sleep(1)
        password.clear()
        password.send_keys("Tisch123")
        time.sleep(1)
        self.driver.find_element_by_name("s1").click()
        time.sleep(1.5)
        self.driver.get(self.url2)

    def sendRaidList(self):
        self.driver.get(self.urlRaid)
        time.sleep(1)
        self.driver.find_element_by_id("raidListMarkAll535").click()
        time.sleep(0.5)
        element = self.driver.find_element_by_id("list535")
        form = element.find_element_by_tag_name("form")
        listContents = form.find_element_by_class_name("listContent")
        buttons = listContents.find_elements_by_tag_name("button")
        buttons[1].click()
        # Ausgeben wieviele raids geschickt wurden
        return_string = driver.find_element_by_tag_name("p")
        print(str(return_string))
    
    pass

# Main loop for TravianBot
driver = webdriver.Chrome()
bot = TravianBot(driver, 1, 5)
bot.login()
time.sleep(1)
bot.sendRaidList()
time.sleep(1)


#driver.close()

