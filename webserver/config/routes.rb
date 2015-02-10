Rails.application.routes.draw do
  get '/streams/bot1', to: 'streams#bot1'
  get '/streams/bot2', to: 'streams#bot2'
end
