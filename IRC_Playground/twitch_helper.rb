require 'pry'
class TwitchHelper
  def self.printReceived(m)
    case m.message
    when /^!left$/
      self.move(m, :left)
    when /^!right$/
      self.move(m, :right)
    when /^!forward$/
      self.move(m, :forward)
    when /^!reverse$/
      self.move(m, :reverse)
    when /^!kick$/
      self.move(m, :kick)
    else
      puts "Invalid Command"
    end
  end

  def self.move(m, command)
    10.times { puts }
    puts m.user.nick + ": " + command.to_s
    10.times { puts }
  end
end
