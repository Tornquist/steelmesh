Rails.application.routes.draw do
  post '/score/', to: 'scores#score'
  resources :scores

  get '/streams/bot1', to: 'streams#bot1'
  get '/streams/bot2', to: 'streams#bot2'
end
