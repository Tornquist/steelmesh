namespace :steelmesh do
  require_relative 'twitch_helper'
  require 'dotenv'
  require 'redis'
  desc "The bot that crawls twitch IRC channels"
  task irc_bot: :environment do
    Dotenv.load

    def new
      bot = Cinch::Bot.new do
        configure do |c|
          c.nick = ENV['TWITCH_USER']
          c.user = ENV['TWITCH_USER']
          c.password = ENV['TWITCH_PW']
          c.server = "irc.twitch.tv"
          c.channels = ["#ece477group1", "#ece477group1_2"]
        end

        redis = Redis.new

        on :message do |m|
          TwitchHelper.parseReceived(redis, m)
        end
      end
      bot.start
    end

    new
  end

end
