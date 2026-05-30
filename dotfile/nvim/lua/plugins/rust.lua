return {
    "mrcjkb/rustaceanvim",
    version = "^6",
    lazy = false,
    init = function()
        vim.g.rustaceanvim = {
            server = {
                on_attach = function(client, bufnr)
                    local map_opts = { buffer = bufnr, silent = true }
                    vim.keymap.set("n", "K", function()
                        vim.cmd.RustLsp({ "hover", "actions" })
                    end, map_opts)
                    vim.keymap.set("n", "gd", function()
                        vim.lsp.buf.definition()
                    end, map_opts)
                    vim.keymap.set("n", "<leader>ca", function()
                        vim.cmd.RustLsp("codeAction")
                    end, map_opts)
                    vim.keymap.set("n", "<leader>rr", "<cmd>RustLsp runnables<CR>", map_opts)
                end,
                default_settings = {
                    ["rust-analyzer"] = {
                        cargo = { buildScripts = { enable = true } },
                        procMacro = { enable = true },
                    },
                },
            },
            dap = {},
        }
    end,
}
