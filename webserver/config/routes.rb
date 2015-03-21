Rails.application.routes.draw do
  post '/score/', to: 'scores#score'
  resources :scores

  get '/streams/bot1', to: 'streams#bot1'
  get '/streams/bot2', to: 'streams#bot2'
  get '/streams/field', to: 'streams#field'

  get '/feeds/:id/start', to: 'feeds#start'
  get '/feeds/:id/image', to: 'feeds#image'
  get '/feeds/:id/end', to: 'feeds#end'
end
