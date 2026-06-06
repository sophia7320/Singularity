return {
    {
        "milanglacier/minuet-ai.nvim",
        config = function()
            require("minuet").setup({
                blink = { enable_auto_complete = false },
                provider = "openai_fim_compatible",
                provider_options = {
                    openai_fim_compatible = {
                        -- 在此填写你的 DeepSeek API Key
                        api_key = "DEEPSEEK_API_KEY",
                        name = "Deepseek",
                        optional = {
                            max_tokens = 256,
                        },
                    },
                },
                virtualtext = {
                    auto_trigger_ft = {},
                    keymap = {
                        -- accept whole completion
                        accept = "<A-A>",
                        -- accept one line
                        accept_line = "<A-a>",
                        -- accept n lines (prompts for number)
                        -- e.g. "A-z 2 CR" will accept 2 lines
                        accept_n_lines = "<A-z>",
                        -- Cycle to prev completion item, or manually invoke completion
                        prev = "<A-[>",
                        -- Cycle to next completion item, or manually invoke completion
                        next = "<A-]>",
                        dismiss = "<A-e>",
                    },
                },
            })
            -- vim.keymap.set("i", "<A-y>", function()
            --     require("blink.cmp").show({ providers = { "minuet" } })
            -- end, { desc = "Minuet AI completion" })
        end,
    },
}
