                   The complete architecture.
                    app_main()
                        │
                        ▼
            button_controller_start()
                        │
             ┌──────────┴──────────┐
             ▼                     ▼
       Button Task            Controller Task
             │                     │
             │                     │
       read GPIO12                 │
             │                     │
       detect press                │
             │                     │
       classify event              │
             │                     │
             ▼                     │
     button_event_callback()       │
             │                     │
             │ xTaskNotify()       │
             └────────────────────►│
                                   │
                            xTaskNotifyWait()
                                   │
                                   ▼
                              switch(event)
                                   │
             ┌─────────────┬───────┼─────────────┐
             ▼             ▼       ▼             ▼
          SINGLE         SHORT   LONG         TRIPLE
             │             │       │             │
             ▼             ▼       ▼             ▼
           RED          YELLOW   YELLOW        GREEN
           blink         blink    solid       × 3