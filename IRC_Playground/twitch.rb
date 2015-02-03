require 'pry'
require 'cinch'
require_relative 'twitch_helper'
require 'dotenv'
Dotenv.load

def new
  bot = Cinch::Bot.new do
    configure do |c|
      c.nick = ENV['TWITCH_USER']
      c.user = ENV['TWITCH_USER']
      c.password = ENV['TWITCH_PW']
      c.server = "irc.twitch.tv"
      c.channels = ["#twitchplayspokemon"]
    end

    on :message do |m|
      TwitchHelper.printReceived(m)
    end
  end
  bot.start
end

new
